#include <iostream>
#include <string>

#include "gpio.h"
#include "sock.h"
#include "serialib.h"


using namespace std;

#ifdef __linux__
#define DEVICE_PORT "/dev/ttyO4"
#endif
 
int main(int argc, char *argv[]) {

	Sock s;
	string reply = "";
	s.conn("192.168.0.105", "1337");
	//s.sendline("tacos are my past");
	do{
	reply = s.readline();
	s.sendline("Command Received");
	cout << reply;
	}while (reply != "q");
	s.clo();

	/*
	GPIO* gpio = GPIO::getInstance();
	int pin = gpio->setup("P9_12", OUTPUT);
	int pin2 = gpio->setup("P9_14", OUTPUT);
	gpio->setValue(pin2, HIGH);

	sleep(5);

	//add serial code here:
	serialib* ser = new serialib();
	ser->Open("/dev/ttyO4", 115200);
	ser->WriteString(reply.c_str());

	sleep(10);

	gpio->setValue(pin2, LOW);
	delete gpio;
	delete ser;
	*/
	return 0;

}

