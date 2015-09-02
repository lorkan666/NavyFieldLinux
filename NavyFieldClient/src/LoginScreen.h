#pragma once
#include <string>

#include "EditView.h"
#include "ImageView.h"
#include "LinearLayout.h"
#include "PasswordView.h"
#include "SimpleButton.h"
#include "TextView.h"
#include "GameScreen.h"
#include "ConnectionListener.h"
#include "PacketListener.h"
using namespace std;

class LoginScreen :
	public GameScreen,
	public ConnectionListener,
	public PacketListener
{
public:
	LoginScreen(void);
	LoginScreen(string name);
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
	~LoginScreen(void);
};

