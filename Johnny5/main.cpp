#include <iostream>
#include <string>

#include "gpio.h"#include "serialib.h"

using namespace std;

#ifdef __linux__
#define DEVICE_PORT "/dev/ttyO4"
#endif
 
int main(int argc, char *argv[]) {
	Sock s;
	s.conn("192.168.0.100", "1337");

	int pin = gpio->setup("P9_12", OUTPUT);	int pin2 = gpio->setup("P9_14", OUTPUT);

	sleep(5);	delete gpio;	return 0;
}