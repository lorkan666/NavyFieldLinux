/*
 * UDPSocket.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#include "UDPSocket.h"
#include "MySockets.h"

UDPSocket::UDPSocket():AbstractSocket(){
}

UDPSocket::UDPSocket(const char* adress, unsigned short port):UDPSocket() {
	service.sin_family = AF_INET;
	service.sin_port = htons( (unsigned short) port);
	if(adress != NULL)
		service.sin_addr.s_addr = inet_addr(adress);
	else
		service.sin_addr.s_addr = INADDR_ANY;

}

int UDPSocket::sendData( const char* data, int size) {
	return 0;
}

int UDPSocket::receiveData(const char* data, int size) {
	return 0;
}

bool UDPSocket::sendData(const Address& destination, const char* data, int size) {
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.GetAddress());
	address.sin_port = htons( (unsigned short) destination.GetPort() );
	int bytes_sended = sendto( sock, (const char*)data,	size, 0, (sockaddr*)&address, sizeof(sockaddr_in) );
	return bytes_sended == size;
}

int UDPSocket::receiveData(Address& sender,const char* data, int size) {
	#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
	#endif
	sockaddr_in from;
	socklen_t fromLength = sizeof( from );
	int received_bytes = recvfrom( sock, (char*)data,	size,  0, (sockaddr*)&from, &fromLength );
	if ( received_bytes <= 0 )
		return 0;

	sender = Address(ntohl(from.sin_addr.s_addr), ntohs(from.sin_port));
	return received_bytes;
}

UDPSocket::~UDPSocket() {
}
