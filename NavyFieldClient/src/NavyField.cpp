
#define MARKUP_STL
#include "Game.h"
#include "LoginScreen.h"
#include "BattleScreen.h"
#include "MenuScreen.h"
#include <SDL/SDL.h>
#include <iostream>
#include <sstream>
#include <MySockets.h>

int main(int argc, char *argv[])
{
	try{
		Game * g = new Game();
		int portArg;
		if(argc>1){
			istringstream iss(argv[1]);
			iss >> portArg;
			g->setIntProperty("client_port",portArg);
		}
		if(g == NULL)
			throw string(SDL_GetError());

		g->addScreen(new BattleScreen("battle"));
		g->addScreen(new MenuScreen("menu"));
		g->addScreen(new LoginScreen("login"));

		g->start();
		SDL_Quit();
		delete g;
	}catch(string &caught){ // not quite sure the syntax is ok here...
		SDL_Quit();
		cout<<"Got: "<<caught.c_str()<<endl;
	}catch(...){
		SDL_Quit();
		cout<<"Got: Unknow Exception"<<endl;
	}

    return 0;
}
