#include <iostream>
#include <string>

#include "gpio.h"

using namespace std;

int main(int argc, char *argv[]) {
	// Get Server IP address
	/*if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <serverIPaddress>" << endl;
		return -1;
	}
	string serverIP = argv[1];*/

	// Homegrown GPIO example
	GPIO *gpio = GPIO::getInstance();
	int pin = gpio->setup("P9_12", OUTPUT);

	gpio->setValue(pin, HIGH);
	cout << "HIGH" << endl;
	
	sleep(5);

	gpio->setValue(pin, LOW);
	cout << "LOW" << endl;

	delete gpio;
	return 0;
}