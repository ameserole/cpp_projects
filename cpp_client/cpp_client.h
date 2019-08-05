#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include <string>
#include <iostream>

#define SOCK_BUFF_LEN 1024

using namespace std;

class cppClient {
private:
	string address;
	int port;

	struct sockaddr_in serv_addr; 
	struct hostent *server;
	int sockfd = 0;

public:
	cppClient(string addr, int p);
	~cppClient();

	string recv();
	string recv(int numBytes);
	string recvLine();
	string recvUntil();

	void send(string buffer);
	void sendLine(string buffer);

	void interactive();
};