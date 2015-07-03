#include "Game.h"

#include <iostream>
#include <sstream>
#include <MyUDPConnection.h>
#include "GameScreen.h"
using namespace std;

Game::Game()
{
	wyjscie = false;
	frames = 0;
	readProperties("config.xml");
	SDL_Init( SDL_INIT_EVERYTHING );  
	SDL_EnableKeyRepeat(200, 100);
	MAX_FPS = getIntProperty("MAX_FPS");
	Uint32 flags = SDL_HWSURFACE ;
	if(getBoolProperty("fullscreen"))
		flags|=SDL_FULLSCREEN;

    gameSurface = SDL_SetVideoMode( getIntProperty("width"), getIntProperty("height"), 32, flags );//| SDL_FULLSCREEN
	if(gameSurface == NULL){
		 SDL_Quit();
		 throw string(SDL_GetError());
	}
	this->width = gameSurface->w;
	this->height = gameSurface->h;	
	currentScreen=screens.begin();

	connection = new MyUDPConnection(getIntProperty("protocol_id"));
	connection->setTimeout(getDoubleProperty("timeout"));
	connection->keepAlive(getBoolProperty("keep_alive"));
	connection->setMode(ConnectionInfo::Client);
	server_address = Address(getStringProperty("server_ip"),(unsigned int)getIntProperty("server_port"));

}

void Game::addScreen(GameScreen* gs){
	if(gs == NULL)
		return;

	screens.push_back(gs);
	currentScreen=--screens.end();
	gs->setGame(this);
}

void Game::start(void){
	if(screens.size() <= 0){
		return;
	}
	connection->start(Address(0,getIntProperty("client_port")));

	SDL_Event event;
	wyjscie = false;
	if(currentScreen != screens.end() && *currentScreen != NULL)
		(*currentScreen)->enter();
	Uint32 frames=0;
	Uint32 startTicks,stopTicks,totalTicks=1;
	int delay=1;
    while( !wyjscie )
    {
		startTicks= SDL_GetTicks();
		if( frames > MAX_FPS && totalTicks > 0){
			stringstream caption;
			caption << "FPS: " << frames*1000/totalTicks;
			SDL_WM_SetCaption(caption.str().c_str(),NULL);
			frames=0;
			totalTicks=0;
		}
        while( SDL_PollEvent( & event ) )
        {			
            if( event.type == SDL_QUIT )
            {
                wyjscie = true;
            }
			if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    (*currentScreen)->onMouseDown(event.button.x,event.button.y, NULL);
            }
			if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                    (*currentScreen)->onMouseUp(event.button.x,event.button.y, NULL);
            }
			if (event.type == SDL_MOUSEMOTION)
            {
				(*currentScreen)->onMouseMove(event.motion.x,event.motion.y, event.motion.xrel, event.motion.yrel, NULL);
            }
			if (event.type == SDL_KEYDOWN)
            {
				(*currentScreen)->onKeyDown(event.key, NULL);
				if(event.key.keysym.sym == SDLK_l){					
					wyjscie = true;
				}					 
            }
			if (event.type == SDL_KEYUP)
            {
                (*currentScreen)->onKeyUp(event.key, NULL);
            }
			if(event.button.button == SDL_BUTTON_WHEELDOWN){
				(*currentScreen)->onMouseWheelDown(event.button.x,event.button.y, NULL);
			}
			if(event.button.button == SDL_BUTTON_WHEELUP){
				(*currentScreen)->onMouseWheelUp(event.button.x,event.button.y, NULL);
			}
        }
		if(screens.size()>0){
			(*currentScreen)->render(gameSurface, delay);
			SDL_Flip( gameSurface );
		}
		
		frames++;
		stopTicks = SDL_GetTicks();
		totalTicks += stopTicks - startTicks;
		delay = 1000/MAX_FPS - (stopTicks - startTicks);
		if(delay<0)
			delay=0;
		if(MAX_FPS > 0 && delay > 0){
			SDL_Delay(delay);
		}
		stopTicks = SDL_GetTicks();
		totalTicks += stopTicks - startTicks;
    }
    SDL_Quit();
}

void Game::nextScreen(){
	(*currentScreen)->leave();
	currentScreen++;
	(*currentScreen)->enter();
}

void Game::previousScreen(){
	(*currentScreen)->leave();
	currentScreen--;
	(*currentScreen)->enter();
}

void Game::changeScreen(string name){
}

void Game::quit(){
	wyjscie = true;
}

Game::~Game(void)
{
	if(connection != NULL)
		delete connection;
	list<GameScreen*>::iterator i;
	for(i = screens.begin(); i != screens.end(); i++ )
	{
		delete *i;
	}
}
