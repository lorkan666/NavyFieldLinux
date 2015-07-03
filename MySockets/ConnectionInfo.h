/*
 * ConnectionInfo.h
 *
 *  Created on: 27 cze 2015
 *      Author: lorkan
 */

#ifndef CONNECTIONINFO_H_
#define CONNECTIONINFO_H_
#include "Address.h"
#include "AckSystem.h"
#include "ConnectionListener.h"
#include <ctime>
#include <iostream>
#include <map>
using namespace std;


class ConnectionInfo : public ConnectionListener{
public:
	enum Mode{
		None,
		Client,
		Server
	};

	enum State{
		Disconnected,
		Connecting,
		ConnectFail,
		Connected
	};

	ConnectionInfo();
	virtual ~ConnectionInfo();
	Address address;
	AckSystem acks_sys;
	void setState(ConnectionInfo::State s);
	void setConnectionListener(ConnectionListener * cl);
	virtual void onConnect(ConnectionInfo &ci);
	virtual void onDisconnect(ConnectionInfo &ci);
	virtual void onConnectFailed(ConnectionInfo &ci);
	float timeoutAccumulator;
	time_t last_send_time;
	State state;
	Mode mode;
private:
	ConnectionListener * cl;
};

#endif /* CONNECTIONINFO_H_ */
