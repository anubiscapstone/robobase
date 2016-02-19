/**
Author:
Matt Hunter
Noah Orr

2/27/15

Use sockets to communicate between hosts.  These objects can receive instuctions from the ANUBIS server for robot motions and can
also send back some queries as well.
**/

#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

#include "sock.h"

using namespace std;

Sock::Sock() {
	memset(&host_info, 0, sizeof(host_info));
	host_info.ai_family = AF_UNSPEC; // can be either ipv4 or ipv6
	host_info.ai_socktype = SOCK_STREAM; // SOCK_STREAM = TCP
}

Sock::~Sock() {
	freeaddrinfo(host_info_list);
	clo();
}

int Sock::conn(char *host, char *port) {
	int status;
	status = getaddrinfo(host, port, &host_info, &host_info_list);
	if (status != 0) return status;

	sockid = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (sockid == -1){
		clo();
		return -1;
	}

	status = connect(sockid, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status != 0){
		clo();
		return status;
	}

	return 0;
}

void Sock::sendline(char *line) {
	ssize_t bytes_sent;

	do {
		int len = strlen(line);
		bytes_sent = send(sockid, line, len, 0);

		string sl(line);
		sl = sl.substr(bytes_sent, sl.size() - bytes_sent);
		line = (char *)sl.c_str();
	} while (strlen(line) > 0);

	send(sockid, "\n", 1, 0); // add \n
}

void Sock::sendline(const char *line) {
	sendline((char *)line);
}

string Sock::readline() {
	ssize_t bytes_received;
	string line = "";

	while (true) {
		char *incomming = new char[1024];
		bytes_received = recv(sockid, incomming, 1024, 0);

		if (bytes_received <= 0) return "";
		line += incomming;
		
		delete[] incomming;

		if (line.find("\n", 0) != string::npos) break;
	}

	return line.substr(0, line.size() - 1); // remove \n
}

void Sock::clo() {
	close(sockid);
}
