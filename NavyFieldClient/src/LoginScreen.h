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
using namespace std;

class LoginScreen :
	public GameScreen,
	public ConnectionListener
{
public:
	LoginScreen(void);
	LoginScreen(string name);
	virtual void render(SDL_Surface *, int );
	virtual void enter();
	virtual void leave();
	virtual void onConnect(ConnectionInfo &ci);
	virtual void onDisconnect(ConnectionInfo &ci);
	virtual void onConnectFailed(ConnectionInfo &ci);
	~LoginScreen(void);
};

