/*
 * PacketListener.h
 *
 *  Created on: 28 cze 2015
 *      Author: lorkan
 */

#ifndef PACKETLISTENER_H_
#define PACKETLISTENER_H_
#include "MyPacket.h"
#include "Address.h"

class PacketListener {
public:
	virtual void onPacketSent(MyPacket p, Address address)=0;
	virtual void onPacketNoneSent(MyPacket p, Address address)=0;
	virtual void onPacketReceived(MyPacket p, Address address)=0;
	virtual void onPacketDelivered(MyPacket p, Address address)=0;
	virtual void onPacketLost(MyPacket p, Address address)=0;
};

#endif /* PACKETLISTENER_H_ */
