#include <iostream>
#include <string>

#include "sock.h"

using namespace std;

int main(int argc, char *argv[]) {
	Sock s;
	s.conn("192.168.0.100", "1337");

	s.sendline("tacos are my past");

	string rec = s.readline();

	cout << rec << endl;
	s.clo();
	return 0;
}