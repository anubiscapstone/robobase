#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>

//#include "model header"

#define PID_PATH "/dev/shm/videocon.pid"
#define SID_PATH "/dev/shm/mjpg_streamer.pid"
#define SCRIPT_PATH "/usr/local/bin/anubisVideo/beginstream.sh"

//ModelType* service;

bool serviceIsRunning();
pid_t getServicePid();
pid_t getStreamiPid();
void putServicePid(pid_t pid);
void start();
void stop();
void status();
void runService();
void stopService(int signum);
void queryService(int signum);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "USAGE: " << argv[0] << " [option]" << std::endl;
		std::cerr << "Options: start, stop, restart, status" << std::endl;
		return 1;
	}

	std::string mode = argv[1];

	if (mode == "start") {
		if (serviceIsRunning()) {
			std::cerr << "Service is already running" << std::endl;
			return 1;
		}
		start();
	}

	else if (mode == "stop") {
		if (!serviceIsRunning()) {
			std::cerr << "Service is not running" << std::endl;
			return 1;
		}
		stop();
	}

	else if (mode == "restart") {
		if (!serviceIsRunning()) {
			std::cerr << "Service is not running" << std::endl;
			return 1;
		}
		stop();
		sleep(2);
		start();
	}

	else if (mode == "status") {
		if (!serviceIsRunning()) {
			std::cerr << "Service is not running" << std::endl;
			return 1;
		}
		status();
	}

	else {
		std::cerr << "Unrecognized mode: " << mode << std::endl;
		return 1;
	}

	return 0;
}

bool serviceIsRunning() { // returns TRUE if the service process is running
	try {
		return (kill(getServicePid(), 0) == 0); // zero sends NO signal, kill's return value of zero means no errors.
	}
	catch (const int e) {
		return false;
	}
}

pid_t getServicePid() { // get the pid of the service process from /dev/shm
	pid_t pid;
	std::ifstream in;
	in.open(PID_PATH);
	if (!in.good()) throw 1;
	in >> pid;
	in.close();
	return pid;
}

pid_t getStreamiPid() { // get the pid of the streaming process from /dev/shm
	pid_t pid;
	std::ifstream in;
	in.open(SID_PATH);
	if (!in.good()) throw 1;
	in >> pid;
	in.close();
	return pid;
}

void putServicePid(pid_t pid) { // store the pid of the service process in /dev/shm
	std::ofstream out;
	out.open(PID_PATH);
	out << pid << std::endl;
	out.close();
}

void start() { // start the service process
	pid_t pid = fork();
	if (pid == 0) runService(); // child
	else if (pid > 0) std::cout << "Started service" << std::endl; // parent
	else std::cerr << "Problem starting service..." << std::endl; // error
}

void stop() { // stop the service process
	kill(getServicePid(), 15); // 15 = SIGTERM
	std::cout << "Stopped service" << std::endl;
}

void status() { // query the service for its current status
	kill(getServicePid(), 14); // 14 = SIGALRM
	sleep(2);
}

void runService() { // call this if you are the service process!

	// Store my pid
	putServicePid(getpid());

	// setup sigterm handler (terminate)
	struct sigaction term;
	memset(&term, 0, sizeof(struct sigaction));
	term.sa_handler = stopService;
	sigaction(SIGTERM, &term, NULL);

	// setup sigalrm handler (query status)
	struct sigaction alrm;
	memset(&alrm, 0, sizeof(struct sigaction));
	alrm.sa_handler = queryService;
	sigaction(SIGALRM, &alrm, NULL);

	// start background service
	system(SCRIPT_PATH);

}

void stopService(int signum) { // sigterm handler
	kill(getStreamiPid(), 15); // 15 = SIGTERM
}

void queryService(int signum) { // sigalrm handler
	std::string cmd = "printf %s 'Videocon is running!' | wall -n";
	system(cmd.c_str());
}
