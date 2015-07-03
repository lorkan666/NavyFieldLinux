#ifndef game_h
#define game_h

#include <SDL/SDL.h>
#include <list>

#include "PropertiesConfigReader.h"
#include "MySockets.h"

using namespace std;

class MyUDPConnection;
class GameScreen;
class Game: public PropertiesConfigReader
{
public:
	MyUDPConnection *connection;
	Address server_address;
	int width, height;
	Game();
	void nextScreen();
	void previousScreen();
	void changeScreen(string name);
	void quit();
	void addScreen(GameScreen*);

	~Game(void);
	void start(void);

private:
	Uint32 frames;
	Uint32 MAX_FPS;

	static Uint32 setCaption (Uint32 intervall, void *frames);
	bool wyjscie;
	list<GameScreen*>::iterator currentScreen;
	SDL_Surface * gameSurface;
	list<GameScreen*> screens;
};

#endif

