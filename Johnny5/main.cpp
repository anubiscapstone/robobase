#include <iostream>
#include <string>

#include "gpio.h"
#include "serialib.h"

using namespace std;

#ifdef __linux__
#define DEVICE_PORT "/dev/ttyO4"
#endif
 
int main(int argc, char *argv[]) {
	// Get Server IP address
	/*if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <serverIPaddress>" << endl;
		return -1;
	}
	string serverIP = argv[1];*/

	// Homegrown GPIO example
	
	serialib SPort;
	int SPortRet;
	char Buffer[128];
	
	SPortRet=SPort.Open(DEVICE_PORT, 115200);
	if (SPortRet != 1){
		cout << "Error Opening Port"<< endl;
		return SPortRet;
		}
		cout << "Port Opened" << endl;
	
	GPIO *gpio = GPIO::getInstance();
	int pin = gpio->setup("P9_12", OUTPUT);
	int pin2 = gpio->setup("P9_14", OUTPUT);

	gpio->setValue(pin, HIGH);
	cout << "Pin 12 HIGH" << endl;
	gpio->setValue(pin2, HIGH);
	cout << "Pin 14 HIGH" << endl;
	
	sleep (2);
	
	SPortRet = SPort.WriteString("#0 P1500 #1 P1500 #2 P1500 #3 P1500 #4 P1500 #5 P1500 #6 P1500 #7 P1500 #8 P1500 #9 P1500 #10 P1500 #11 P1500 #12 P1500 #13 P1500 #14 P1500 #15 P1500\r");
	if (SPortRet != 1){
		cout << "Error Writing Port"<< endl;
		return SPortRet;
		}
		cout << "Port Done" << endl;
	
	SPortRet=SPort.ReadString(Buffer,'\r',128,5000);                                // Read a maximum of 128 characters with a timeout of 5 seconds
                                                                            // The final character of the string must be a line feed ('\n')
    if (SPortRet>0)                                                              // If a string has been read from, print the string
        cout<< "String read from serial port "<<endl;
    else
        cout<< "TimeOut reached. No data received !\n" << endl; 
	
	sleep(10);
	
	
	SPort.Close();
	
	gpio->setValue(pin, LOW);
	cout << "Pin 12 LOW" << endl;
	gpio->setValue(pin2, LOW);
	cout << "Pin 14 LOW" << endl;

	delete gpio;
	return 0;
}