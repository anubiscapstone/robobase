#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdlib>

#include "gpio.h"
#include "serialib.h"

using namespace std;

void registerSigaction();
void writepid();
void terminate(int signum);

int servo_pin;

int main(int argc, char *argv[]) {
	registerSigaction();
	writepid();

	GPIO *gpio = GPIO::getInstance();
	servo_pin = gpio->setup("P9_14", OUTPUT);
	gpio->setValue(servo_pin, HIGH);

	sleep(1);

	const char *iv = "#0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500 #16 P1500\r";

	serialib *ser = new serialib();
	ser->Open("/dev/ttyO4", 115200);
	ser->WriteString(iv);
	ser->Close();
	delete ser;

	while (1) { }
}

void registerSigaction() {
	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = terminate;
	sigaction(SIGTERM, &action, NULL);
}

void writepid() {
	ofstream out;
	out.open("/dev/shm/Johnny5pid");
	out << getpid() << endl;
	out.close();
}

void terminate(int signum) {
	GPIO *gpio = GPIO::getInstance();
	gpio->setValue(servo_pin, LOW);
	gpio->clean();

	delete gpio;
	exit(0);
}
