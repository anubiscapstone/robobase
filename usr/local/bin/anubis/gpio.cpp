/**
Author:
Matt Hunter
Noah Orr

9/24/14

GPIO interface library.  Uses Sysfs GPIO API under the hood.

To use: use getInstance to get the GPIO object.  Use GPIO.setup to get a single pin.
getValue can read an input pin.  setValue sets the output of an output pin.
When you are done with the pins, you may simply delete the GPIO object.

For more information on the internal workings, search "GPIO sysfs interface".  This is a standard linux system.
**/


#include <fstream>
#include <string.h>
#include <stdio.h>

#include "gpio.h"
#include "gpio_pintable.h"

using namespace std;

GPIO * GPIO::instance = 0;
const string BASEPATH = "/sys/class/gpio";

GPIO::GPIO() {
}

GPIO::~GPIO() {
	clean();
	instance = 0; // set instance back to NULL as to NOT break getInstance() func
}

int GPIO::getGpioByKey(const char *key) {
	pins_t *p;
	for (p = pinTable; p->key != NULL; ++p) {
		if (strcmp(p->key, key) == 0) {
			return p->gpio;
		}
	}

	return 0;
}

GPIO * GPIO::getInstance() {
	if (instance == 0) instance = new GPIO();
	return instance;
}

int GPIO::setup(const char *key, DIRECTION direction) {
	int pin = getGpioByKey(key);
	char spin[15];
	sprintf(spin, "%d", pin);

	ofstream fs;

	fs.open((BASEPATH + "/export").c_str());
	fs << pin;
	fs.close();

	fs.open((BASEPATH + "/gpio" + spin + "/direction").c_str());
	fs << (direction == INPUT ? "in" : "out");
	fs.close();

	pinsInUse.push_back(pin);

	return pin;
}

int GPIO::setValue(unsigned int pin, PIN_VALUE value) {
	char spin[15];
	sprintf(spin, "%d", pin);

	ofstream fs;
	fs.open((BASEPATH + "/gpio" + spin + "/value").c_str());
	fs << value;
	fs.close();

	return 0;
}

PIN_VALUE GPIO::getValue(unsigned int pin) {
	char spin[15];
	sprintf(spin, "%d", pin);

	ifstream fs;
	fs.open((BASEPATH + "/gpio" + spin + "/value").c_str());
	int value;
	fs >> value;
	fs.close();
	
	return (value == 0 ? LOW : HIGH);
}

void GPIO::clean() {
	ofstream fs;
	fs.open((BASEPATH + "/unexport").c_str());

	for (int i = 0; i < pinsInUse.size(); i++) {
		int pin = pinsInUse[i];
		fs << pin;
	}

	fs.close();
}
