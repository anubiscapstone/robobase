#ifndef bbb_anubis
#define bbb_anubis

#include <string>

#include "sock.h"
#include "gpio.h"
#include "serialib.h"

using namespace std;

class Anubis {
private:
	bool running;
	bool connected;
	int servo_pin;
	int treds_pin;

	Sock *socket;
	GPIO *gpio;
	serialib *serial;

public:
	Anubis();
	~Anubis();

	void start();
	void stop();
	string getStatus();
};

#endif // bbb_anubis
