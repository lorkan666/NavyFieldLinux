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
#include "PlayerInfoList.h"
#include "GameProtocol.h"

class Portier: public PropertiesConfigReader, public MyUDPConnection {
public:
	Portier();
	virtual void onPacketSent(MyPacket p, Address address);
	virtual void onPacketNoneSent(MyPacket p, Address address);
	virtual void onPacketReceived(MyPacket p, Address address);
	virtual void onPacketDelivered(MyPacket p, Address address);
	virtual void onPacketLost(MyPacket p, Address address);
	virtual ~Portier();
private:
	PlayerInfoList loged_players;
	bool isPlayerExist(LoginPacket lp);
};

#endif /* PORTIER_H_ */
