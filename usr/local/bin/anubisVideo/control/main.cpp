/**
Author:
Matt Hunter

2/27/15

Entry-point to Video Control.   Logic in this file is used for process mgmt. and spawns the background process (or kills it, etc.).
**/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

//#include "model header"

#define PID_NAME "mjpg_streamer"
#define SCRIPT_PATH "/usr/local/bin/anubisVideo/beginstream.sh > /dev/null 2>&1 &"

//ModelType* service;
using namespace std;

bool serviceIsRunning();
int getStreamPid();
void start();
void stop();

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "USAGE: " << argv[0] << " [option]" << endl;
		cerr << "Options: start, stop, restart, status" << endl;
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
		else{
			cout << "Videocon is running!" << endl;
		}
	}

	else {
		cerr << "Unrecognized mode: " << mode << endl;
		return 1;
	}

	return 0;
}

bool serviceIsRunning() { // returns TRUE if the service process is running
	try {
		int pid = getStreamPid();
		return (pid >= 0) && (kill((pid_t) pid, 0) == 0); // zero sends NO signal, kill's return value of zero means no errors.
	}
	catch (const int e) {
		return false;
	}
}


int getStreamPid() { // get the pid of the streaming process from /proc
	int pid = -1;

	DIR* dp = opendir("/proc");
	if(dp != NULL){
		struct dirent* dirp;
		while(pid < 0 && (dirp = readdir(dp))){
			int id = atoi(dirp->d_name);
			if(id > 0){
				string cmdPath = string("/proc/") + dirp->d_name + "/comm";
				ifstream cmdFile(cmdPath.c_str());
				string cmdLine;
				getline(cmdFile, cmdLine);
				if(!cmdLine.empty()){
					size_t pos = cmdLine.find('\0');
					if(pos != string::npos)
						cmdLine = cmdLine.substr(0, pos);
					pos = cmdLine.find(PID_NAME);
					if(pos != string::npos)
						cmdLine = cmdLine.substr(pos);
					if(PID_NAME == cmdLine)
						pid = id;
				}
			}
		}
	}
	closedir(dp);

	return pid;
}

void start() { // start the service process
	// start background service
	system(SCRIPT_PATH);
	cout << "Started service" << endl;
}

void stop() { // stop the service process
	kill(getStreamPid(), 15); // 15 = SIGTERM
	cout << "Stopped service" << endl;
}
