/*
The ANUBIS client software runs as a background service and attempts to connect to the server detailed in
'/etc/anubis/host'.     While connected, this software listens for RAW serial instruction strings from the
server and then repeats them to the robot conencted via serial interface locally.

If the connection is dropped, this software attempts to reconenct until connection is established OR this
process is killed.


When the process starts, this software sets up the GPIO pins to enable the servo and tred motors.  Initially,
and while not conencted to a server, the position for all motors is set to their mid-point (P1500), with treds
turned off.

When the process ends, this software cleans up all GPIO and serial assets before terminating.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "anubis.h"
#include "sock.h"
#include "gpio.h"
#include "serialib.h"

using namespace std;

const char *hostfile = "/etc/anubis/host";
const char *helofile = "/etc/anubis/helo";
const char *serdev = "/dev/ttyO4";
const int baud = 115200;
const char *iv = "#0 P1500 #1 P1550 #2 P1400 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500 #16 P1500\r";
const char *nv = "#0L #1L #2L #3L #4L #5L #6L #7L #8L #9L #10L #11L #12L #13L #14 P1500 #15 P1500 #16L\r";
const char *pinname_servo = "P9_14";
const char *pinname_treds = "P9_12";

Anubis::Anubis() {

	// Init obj vars
	running = false;
	connected = false;
	verbose = false;
	servo_pin = 0;
	treds_pin = 0;
	servo_vector = "";

	// Init obj pointers
	socket = new Sock();
	gpio = GPIO::getInstance();
	serial = new serialib();

	// Init GPIO
	servo_pin = gpio->setup(pinname_servo, OUTPUT);
	treds_pin = gpio->setup(pinname_treds, OUTPUT);

}

Anubis::~Anubis() {
}

void Anubis::start() {

	// Set running flag
	running = true;

	// Power on servos
	gpio->setValue(servo_pin, HIGH);

	// Wait for SSC-32 to boot
	sleep(1);

	// Open serial port
	serial->Open(serdev, baud);

	/* MAIN LOOP - maintain network connection and do what it says */
	while (running) {
		// Reset Hardware
		applyVector(iv);
		gpio->setValue(treds_pin, LOW);

		// Connect to server
		connected = false;
		connToServer();
		if (!running) break;
		connected = true;

		// Identify myself to server
		heloToServer();

		// Activate Treds
		gpio->setValue(treds_pin, HIGH);

		// Accept instructions from server
		acceptServerMsgs();
	}

	// Close network socket
	socket->clo();

	// Send the nv to the robot
	applyVector(nv);
	serial->Close();

	// Power off motors
	gpio->setValue(servo_pin, LOW);
	gpio->setValue(treds_pin, LOW);
	gpio->clean();

}

void Anubis::stop() {
	// Stop main loop
	running = false;
}

string Anubis::getStatus() {
	stringstream buff;

	buff << "Anubis Control Status" << endl;
	buff << "Running: " << (running ? "TRUE" : "FALSE") << endl;
	buff << "Server: " << (connected ? "CONNECTED" : "DISCONNECTED") << endl;

	bool servos = gpio->getValue(servo_pin) == HIGH;
	bool treds = gpio->getValue(treds_pin) == HIGH;
	buff << "Servos: " << (servos ? "ON" : "OFF") << endl;
	buff << "Treds: " << (treds ? "ON" : "OFF") << endl;

	buff << "Verbose O/P: " << (verbose ? "ON" : "OFF") << endl;

	return buff.str();
}

void Anubis::connToServer() {
	string host, port;
	char *host_, *port_;
	int status;

	ifstream in;
	in.open(hostfile);
	in >> host;
	in >> port;
	in.close();

	host_ = new char[host.size() + 1];
	port_ = new char[port.size() + 1];
	strcpy(host_, host.c_str());
	strcpy(port_, port.c_str());

	do status = socket->conn(host_, port_);
	while (running && (status != 0));
}

void Anubis::heloToServer() {
	string helo;
	char *helo_;

	ifstream in;
	in.open(helofile);
	in >> helo;
	in.close();

	helo_ = new char[helo.size() + 1];
	strcpy(helo_, helo.c_str());

	socket->sendline(helo_);
}

void Anubis::acceptServerMsgs() {
	while (running) {

		// Get server message
		string line = socket->readline();

		// verbose
		if (verbose) cout << "start new line:" << endl;

		// Lose connection?
		if (line.size() == 0) {
			socket->clo();
			break;
		}

		// Bad request?
		if (line.size() < 2) {
			socket->sendline("err");
			continue;
		}

		// Get Request code
		string rqc = line.substr(0, 2);

		// Store Vector
		if (rqc == "sv") {
			string vec = line.substr(3, line.size() - 3);
			applyVector(vec);
			if (verbose) cout << vec;
		}

		// Ping
		else if (rqc == "pg") {
			socket->sendline("pg"); // pong!
		}

		// Request Vector
		else if (rqc == "rv") {
			char *vec_ = new char[servo_vector.size() + 1];
			strcpy(vec_, servo_vector.c_str());
			socket->sendline(vec_);
			cerr << "anubis.cpp | deleting vec_" << endl;
			delete vec_;
		}

		// Request Data
		else if (rqc == "rd") {
			socket->sendline("nc");
		}

		// Bad Request?
		else {
			socket->sendline("err");
		}
	}
}

void Anubis::applyVector(string vec) {
	servo_vector = vec;
	serial->WriteString(servo_vector.c_str());
}

void Anubis::toggleVerbose() {
	verbose = !verbose;
}
