#include "GameScreen.h"

#include <iostream>

GameScreen::GameScreen()
{
	game = NULL;
	stage = new ViewGroup();
}

GameScreen::GameScreen(string name):GameScreen()
{
	this->name = name;
}

void GameScreen::setGame(Game* g){
	this->game = g;
	stage->resize(game->width,game->height);
}

void GameScreen::enter(){
}

void GameScreen::leave(){
}

void GameScreen::onKeyDown(SDL_KeyboardEvent key , View * v){
	stage->onKeyDown(key,stage);
}

void GameScreen::onKeyUp(SDL_KeyboardEvent key, View * v){
	stage->onKeyUp(key,stage);
}

void GameScreen::onMouseDown(int x, int y, View* v){
	stage->onMouseDown(x,y,stage);
}

void GameScreen::onMouseUp(int x, int y, View* v){
	stage->onMouseUp(x,y,stage);
}

void GameScreen::onMouseEnter(int x, int y, View* v){	
}

void GameScreen::onMouseLeave(int x, int y, View* v){
}

void GameScreen::onClick(int x, int y, View* v){
}

void GameScreen::onMouseWheelDown(int x, int y, View * v){
	stage->onMouseWheelDown(x,y,stage);
}
void GameScreen::onMouseWheelUp(int x, int y, View * v){
	stage->onMouseWheelUp(x,y,stage);
}


void GameScreen::onMouseMove(int x, int y, int xr, int yr, View * v){
	stage->onMouseMove(x,y,xr,yr,stage);
}

void GameScreen::render(SDL_Surface * display, int delay){
	//cout<<"Before"<<endl;
	stage->draw(display, delay);
	//cout<<"After"<<endl;
}

GameScreen::~GameScreen(void)
{
	delete stage;
}
