#pragma once
#include "View.h"

class Ship;
class GameMap;
class PlayerControler: public OnMouseEventListener, public OnKeyboardEventListener
{
public:
	virtual void onMouseDown(int x, int y, View * v) ;
	virtual void onClick(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onMouseMove(int x, int y, int xr, int yr, View * v) ;
	virtual void onMouseEnter(int x, int y, View * v) ;
	virtual void onMouseLeave(int x, int y, View * v) ;
	virtual void onKeyDown(SDL_KeyboardEvent key , View * v);
	virtual void onKeyUp(SDL_KeyboardEvent key, View * v);
	virtual void onMouseWheelDown(int x, int y, View * v);
	virtual void onMouseWheelUp(int x, int y, View * v);

	PlayerControler(GameMap * gm);
	~PlayerControler(void);
private:
	Ship * player;
	GameMap * gm;
};

