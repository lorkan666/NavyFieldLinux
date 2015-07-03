/*
 * TCPSocket.h
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_
#include "AbstractSocket.h"

class TCPSocket: public AbstractSocket
{
public:
	TCPSocket();
	TCPSocket(const char* adress, unsigned short port);
	virtual int sendData( const char * data,int size );
	virtual int receiveData(const char * data, int size );
	virtual bool sendData(const Address & destination, const char * data,int size );
	virtual int receiveData( Address & sender,const char * data, int size );
	virtual ~TCPSocket();
};

#endif /* TCPSOCKET_H_ */
