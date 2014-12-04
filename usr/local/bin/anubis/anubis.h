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
	bool verbose;
	int servo_pin;
	int treds_pin;

	string servo_vector;

	Sock *socket;
	GPIO *gpio;
	serialib *serial;

	void connToServer();
	void heloToServer();
	void acceptServerMsgs();
	void applyVector(string vec);

public:
	Anubis();
	~Anubis();

	void start();
	void stop();
	string getStatus();
	void toggleVerbose();
};

#endif // bbb_anubis
