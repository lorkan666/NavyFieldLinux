/*
 * ConnectionListener.h
 *
 *  Created on: 28 cze 2015
 *      Author: lorkan
 */

#ifndef CONNECTIONLISTENER_H_
#define CONNECTIONLISTENER_H_
#include "ConnectionInfo.h"

class ConnectionInfo;
class ConnectionListener {
public:
	virtual void onConnect(ConnectionInfo &ci)=0;
	virtual void onDisconnect(ConnectionInfo &ci)=0;
	virtual void onConnectFailed(ConnectionInfo &ci)=0;
};

#endif /* CONNECTIONLISTENER_H_ */
