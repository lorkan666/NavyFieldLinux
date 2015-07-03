#pragma once
#include <string>
using namespace std;
#include "GameScreen.h"

class LoginScreen :
	public GameScreen
{
public:
	LoginScreen(void);
	LoginScreen(string name);
	virtual void render(SDL_Surface *, int );
	virtual void enter();
	~LoginScreen(void);
};

