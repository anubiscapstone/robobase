#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <signal.h>
#include <unistd.h>

#include "anubis.h"

using namespace std;

const char *pidpath = "/dev/shm/anubispid";
const char *statpath = "/dev/shm/anubisstatus";
Anubis *service;

bool serviceIsRunning();
pid_t getServicePid();
void putServicePid(pid_t pid);
void start();
void stop();
void status();
void verbose();
void runService();
void stopService(int signum);
void queryService(int signum);
void toggleVerbose(int signum);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cerr << "USAGE: " << argv[0] << " [option]" << endl;
		cerr << "Options: start, stop, restart, status, verbose, service (don't invoke this directly)" << endl;
		return 1;
	}

	string mode = argv[1];

	if (mode == "start") {
		if (serviceIsRunning()) {
			cerr << "Service is already running" << endl;
			return 1;
		}
		start();
	}

	else if (mode == "stop") {
		if (!serviceIsRunning()) {
			cerr << "Service is not running" << endl;
			return 1;
		}
		stop();
	}

	else if (mode == "restart") {
		if (!serviceIsRunning()) {
			cerr << "Service is not running" << endl;
			return 1;
		}
		stop();
		sleep(2);
		start();
	}

	else if (mode == "status") {
		if (!serviceIsRunning()) {
			cerr << "Service is not running" << endl;
			return 1;
		}
		status();
	}

	else if (mode == "verbose") {
		if (!serviceIsRunning()) {
			cerr << "Service is not running" << endl;
			return 1;
		}
		verbose();
	}

	else if (mode == "service") {
		runService();
	}

	else {
		cerr << "Unrecognized mode: " << mode << endl;
		return 1;
	}

	return 0;
}

bool serviceIsRunning() { // returns TRUE if the service process is running
	return (kill(getServicePid(), 0) == 0); // zero sends NO signal, kill's return value of zero means no errors.
}

pid_t getServicePid() { // get the pid of the service process from /dev/shm
	pid_t pid;
	ifstream in;
	in.open(pidpath);
	in >> pid;
	in.close();
	return pid;
}

void putServicePid(pid_t pid) { // store the pid of the service process in /dev/shm
	ofstream out;
	out.open(pidpath);
	out << pid << endl;
	out.close();
}

void start() { // start the service process
	pid_t pid = fork();
	if (pid == 0) runService(); // child
	else if (pid > 0) cout << "Started service" << endl; // parent
	else cerr << "Problem starting service..." << endl; // error
}

void stop() { // stop the service process
	kill(getServicePid(), 15); // 15 = SIGTERM
	cout << "Stopped service" << endl;
}

void status() { // query the service for its current status
	kill(getServicePid(), 14); // 14 = SIGALRM
	sleep(2);

	ifstream in;
	stringstream buff;
	in.open(statpath);
	buff << in.rdbuf();
	in.close();

	cout << buff.str();
}

void verbose() { // toggle verbose output on/off
	kill(getServicePid(), 2); // 2 = SIGINT
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

	// setup sigint handler (toggle verbose output)
	struct sigaction _int;
	memset(&_int, 0, sizeof(struct sigaction));
	_int.sa_handler = toggleVerbose;
	sigaction(SIGINT, &_int, NULL);

	// start background service
	service = new Anubis();
	service->start();

}

void stopService(int signum) { // sigterm handler
	service->stop();
}

void queryService(int signum) { // sigalrm handler
	string status = service->getStatus();

	ofstream out;
	out.open(statpath);
	out << status;
	out.close();
}

void toggleVerbose(int signum) { // sigint handler
	service->toggleVerbose();
}
