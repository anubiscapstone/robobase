#ifndef bbb_sock
#define bbb_sock

#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

using namespace std;

class Sock {
private:
	int sockid;
	addrinfo host_info;
	addrinfo *host_info_list;

public:
	Sock();
	~Sock();

	int conn(char *host, char *port);
	void sendline(char *line);
	string readline();
	void clo();
};

#endif // bbb_sock