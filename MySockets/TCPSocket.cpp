/*
 * TCPSocket.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#include "TCPSocket.h"
#include "MySockets.h"

TCPSocket::TCPSocket():AbstractSocket() {
}

TCPSocket::TCPSocket(const char* adress, unsigned short port):TCPSocket() {
	service.sin_family = AF_INET;
	service.sin_port = htons( (unsigned short) port);
	if(adress != 0)
		service.sin_addr.s_addr = inet_addr(adress);
	else
		service.sin_addr.s_addr = INADDR_ANY;
}

int TCPSocket::sendData( const char* data, int size) {
	return send( sock, (const char*)data,	size, 0 );
}

int TCPSocket::receiveData(const char* data, int size) {
	return recv( sock, (char*)data,	size,  0);
}

bool TCPSocket::sendData(const Address& destination, const char* data, int size) {
	return false;
}

int TCPSocket::receiveData(Address& sender,const char* data, int size) {
	return 0;
}


TCPSocket::~TCPSocket() {
}

