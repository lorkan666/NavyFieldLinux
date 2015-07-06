/*
 * Portier.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef PORTIER_H_
#define PORTIER_H_

#include <PropertiesConfigReader.h>
#include <MyUDPConnection.h>
#include "ConnectionInfoList.h"
#include "ConnectionListener.h"
#include "GameProtocol.h"

class Portier: public PropertiesConfigReader, public MyUDPConnection, public ConnectionListener {
public:
	Portier();
	virtual void onPacketSent(MyPacket p, Address address);
	virtual void onPacketNoneSent(MyPacket p, Address address);
	virtual void onPacketReceived(MyPacket p, Address address);
	virtual void onPacketDelivered(MyPacket p, Address address);
	virtual void onPacketLost(MyPacket p, Address address);
	virtual void onConnect(ConnectionInfo &ci);
	virtual void onDisconnect(ConnectionInfo &ci);
	virtual void onConnectFailed(ConnectionInfo &ci);
	virtual ~Portier();
private:
	bool isPlayerExist(string name, string hashed_password);
};

#endif /* PORTIER_H_ */
