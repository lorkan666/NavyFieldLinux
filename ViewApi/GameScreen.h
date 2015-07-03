#ifndef game_screen_h
#define game_screen_h

#include <SDL/SDL.h>
#include <string>
#include "Game.h"
#include "OnKeyboardEventListener.h"
#include "OnMouseEventListener.h"
#include "ViewGroup.h"

class Game;
class GameScreen:  public OnMouseEventListener, public OnKeyboardEventListener
{
public:
	ViewGroup* stage;
	string name;
	GameScreen(void);
	GameScreen(string name);
	virtual ~GameScreen(void);
	virtual void enter();
	virtual void leave();
	virtual void render(SDL_Surface *, int );
	virtual void onMouseDown(int x, int y, View * v);
	virtual void onClick(int x, int y, View * v);
	virtual void onMouseUp(int x, int y, View * v);
	virtual void onMouseMove(int x, int y, int xr, int yr, View * v);
	virtual void onMouseEnter(int x, int y, View * v);
	virtual void onMouseLeave(int x, int y, View * v);
	virtual void onMouseWheelDown(int x, int y, View * v);
	virtual void onMouseWheelUp(int x, int y, View * v);

	virtual void onKeyDown(SDL_KeyboardEvent key , View * v);
	virtual void onKeyUp(SDL_KeyboardEvent key, View * v);
	void setGame(Game*);

protected:
	Game* game;
	
};

#endif

