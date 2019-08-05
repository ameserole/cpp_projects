#include "cpp_client.h"

cppClient::cppClient(string addr, int p) {
	address = addr;
	port = p;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(port); 

    server = gethostbyname(address.c_str());
    serv_addr.sin_addr.s_addr = *((unsigned long *) server->h_addr_list[0]);

    struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
}

cppClient::~cppClient() {
	close(sockfd);
}

void cppClient::send(string buffer) {
	int n = 0;
	n = ::send(sockfd, buffer.c_str(), buffer.length(), 0);
}

void cppClient::sendLine(string buffer) {
	buffer += "\n";
	send(buffer);
}

string cppClient::recv(int numBytes) {
	char inBuffer[SOCK_BUFF_LEN + 1] = {0};
	int totalBytes = 0;
	string totalBuffer = "";
	int valRead = 0;

	while(totalBytes < numBytes) {
		if((numBytes - totalBytes) < SOCK_BUFF_LEN) 
			valRead = ::recv(sockfd , inBuffer, (numBytes - totalBytes), 0);
		else
			valRead = ::recv(sockfd , inBuffer, SOCK_BUFF_LEN, 0);

		string temp(inBuffer);
		totalBuffer += temp;
		totalBytes += valRead;

		if(valRead < SOCK_BUFF_LEN)
			break;
	}

	return totalBuffer;	
}

string cppClient::recv() {
	char inBuffer[SOCK_BUFF_LEN + 1] = {0};
	string totalBuffer = "";
	int valRead = 1;

	while(valRead > 0) {
		memset(inBuffer, 0, SOCK_BUFF_LEN);
		valRead = ::recv(sockfd , inBuffer, SOCK_BUFF_LEN, 0);

		string temp(inBuffer);
		totalBuffer += temp;

		if(valRead < SOCK_BUFF_LEN)
			return totalBuffer;
	}

	return totalBuffer;	
}

string cppClient::recvUntil(string bytes) {
	string byte = "";
	string totalBuffer = "";
	while(totalBuffer.find(bytes) == string::npos) {
		byte = cppClient::recv(1);
		totalBuffer += byte;
	}

	return totalBuffer;
}

string cppClient::recvLine() {
	return recvUntil("\n");
}

void cppClient::termInput() {
	string userIn = "";

	while(true) {
		cout << "=> ";
		getline(cin, userIn);
		cppClient::sendLine(userIn);
	}
}

void cppClient::termOutput() {
	string termOut = "";

	while(true) {
		termOut = cppClient::recv();
		if(termOut != "")
			cout << termOut << endl;
	}
}

void cppClient::interactive() {
	thread termIn(&cppClient::termInput, this);
	thread termOut(&cppClient::termOutput, this);

	termIn.join();
	termOut.join();
}