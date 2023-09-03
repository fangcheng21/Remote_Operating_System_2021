#ifndef _MATLABCONNECTION_H_
#define _MATLABCONNECTION_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>



//#define MAXLINE 4096

class MatConnection
{
public:

	MatConnection();

	~MatConnection();

	bool open();

	void close();

	bool isOpen() const;

	int receive(char *buffer,int maxSize);

	bool sendto(const char* buffer, int size);


private:
	int sockfd;
	struct sockaddr_in servaddr;
	//unsigned int _receiveTimeout;
};

#endif


