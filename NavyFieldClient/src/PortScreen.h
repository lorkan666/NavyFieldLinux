/*
 * PortScreen.h
 *
 *  Created on: 2 wrz 2015
 *      Author: lorkan
 */

#ifndef PORTSCREEN_H_
#define PORTSCREEN_H_

#include <GameScreen.h>
#include "ConnectionListener.h"
#include "PacketListener.h"

class PortScreen:
	public GameScreen,
	public ConnectionListener,
	public PacketListener{
public:
	PortScreen(void);
	PortScreen(string name);
	virtual void render(SDL_Surface *, int );
	virtual void enter();
	virtual void leave();
	virtual void onConnect(ConnectionInfo &ci);
	virtual void onConnecting(ConnectionInfo &ci);
	virtual void onDisconnect(ConnectionInfo &ci);
	virtual void onConnectFailed(ConnectionInfo &ci);
	virtual void onPacketSent(MyPacket p, Address address);
	virtual void onPacketNoneSent(MyPacket p, Address address);
	virtual void onPacketReceived(MyPacket p, Address address);
	virtual void onPacketDelivered(MyPacket p, Address address);
	virtual void onPacketLost(MyPacket p, Address address);
	int selected_ship;
};

#endif /* PORTSCREEN_H_ */
