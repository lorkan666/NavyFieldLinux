/*
 * SocketFactories.h
 *
 *  Created on: 24 cze 2015
 *      Author: lorkan
 */

#ifndef SOCKETFACTORIES_H_
#define SOCKETFACTORIES_H_
#include "AbstractSocketFactory.h"
#include "TCPSocket.h"
#include "UDPSocket.h"

class SocketFactories{
public:
	class TCPSocketFactory: public AbstractSocketFactory {
	public:
		TCPSocketFactory() {
		}

		virtual AbstractSocket* createSocket(const char* adress, unsigned short port){
			AbstractSocket * sock = new TCPSocket(adress,port);
			return sock;
		}
	};

	class UDPSocketFactory: public AbstractSocketFactory {
	public:
		UDPSocketFactory() {
		}

		virtual AbstractSocket* createSocket(const char* adress, unsigned short port){
			AbstractSocket * sock =new UDPSocket(adress,port);
			return sock;
		}

	};
};



#endif /* SOCKETFACTORIES_H_ */
