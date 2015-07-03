/*
 * UDPSocket.h
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include "AbstractSocket.h"


class UDPSocket: public AbstractSocket {
public:
	UDPSocket();
	UDPSocket(const char* adress, unsigned short port);
	virtual int sendData( const char * data,int size );
	virtual int receiveData(const char * data, int size );
	virtual bool sendData(const Address & destination, const char * data,int size );
	virtual int receiveData( Address & sender,const char * data, int size );
	virtual ~UDPSocket();
};

#endif /* UDPSOCKET_H_ */
