/*
 * AbstractSocket.h
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#ifndef ABSTRACTSOCKET_H_
#define ABSTRACTSOCKET_H_

#include "Address.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include "stdio.h"
using namespace std;

class AbstractSocket {
protected:
	sockaddr_in service;
	int sock;
public:
	AbstractSocket() {
		sock = 0;
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = INADDR_ANY;
		service.sin_port = htons( (unsigned short) 0 );
	}

	void setPort(unsigned short port){
		service.sin_port = htons( (unsigned short) port );
	}

	void setAddress(unsigned int addr){
		service.sin_addr.s_addr = htonl( (unsigned int) addr );
	}

	bool openSocket(){
		sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(sock<=0){
			perror("open failed. Error");
			sock=0;
			return false;
		}
		return true;
	}

	bool bindSocket() {
		if ( bind( sock, (const sockaddr*) &service, sizeof(sockaddr_in) ) < 0 ){
			perror("bind failed. Error");
			closeSocket();
			return false;
		}
		return true;
	}

	bool setBlocking(bool blocking) {
		#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
			if ( fcntl( sock, F_SETFL, O_NONBLOCK, blocking ) == -1 )
			{
				perror("blocking failed. Error");
				closeSocket();
				return false;
			}

		#elif PLATFORM == PLATFORM_WINDOWS
			if ( ioctlsocket( sock, FIONBIO, &blocking ) != 0 )
			{
				perror("blocking failed. Error");
				closeSocket();
				return false;
			}

		#endif

		return true;
	}

	void closeSocket() {
		if ( sock != 0 ){
			#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
				close( sock );
			#elif PLATFORM == PLATFORM_WINDOWS
				closesocket( sock );
			#endif
			sock = 0;
		}
	}

	~AbstractSocket() {
		closeSocket();
	}

	virtual int sendData( const char * data,int size )= 0;
	virtual int receiveData(const char * data, int size ) = 0;
	virtual bool sendData(const Address & , const char * data,int size )= 0;
	virtual int receiveData( Address & ,const char * data, int size ) = 0;
};

#endif /* ABSTRACTSOCKET_H_ */
