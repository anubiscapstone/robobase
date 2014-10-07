#include <iostream>
#include <string>

#include "gpio.h"
#include "sock.h"
#include "serialib.h"

using namespace std;

int main(int argc, char *argv[]) {
	string reply = "";
	GPIO* gpio = GPIO::getInstance();
	int pin = gpio->setup("P9_12", OUTPUT);
	int pin2 = gpio->setup("P9_14", OUTPUT);
	gpio->setValue(pin2, HIGH);
	sleep(1);
	//add serial code here:
	serialib* ser = new serialib();
	ser->Open("/dev/ttyO4", 115200);
	ser->WriteString("#0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500 #16 P1500\r");
	sleep(1);
	gpio->setValue(pin, HIGH);

	Sock s;
        s.conn("192.168.0.105", "1337");
        //s.sendline("tacos are my past");
        do{
        reply = s.readline();
        s.sendline("Command Received");
        cout << reply;
	ser->WriteString(reply.c_str());
        }while (reply != "q");
        s.clo();

	sleep(1);

	gpio->setValue(pin, LOW);
	gpio->setValue(pin2, LOW);
	delete gpio;
	delete ser;
	return 0;

}
