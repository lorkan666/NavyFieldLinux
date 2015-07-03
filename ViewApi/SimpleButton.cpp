#include "SimpleButton.h"


SimpleButton::SimpleButton(string t):TextView(t)
{
	setPadding(2,10,0,10);
	setButtonColor(200,200,200,1);
	setButtonColorDown(150,150,150,1);
	setButtonColorOver(150,200,200,1);
	setBackgroundColor(this->normalColor);
}

void SimpleButton::setButtonColor(int r,int g,int b,int a){
	this->normalColor = SDL_MapRGBA(SDL_GetVideoSurface()->format, r, g, b, a);	
}

void SimpleButton::setButtonColorDown(int r,int g,int b,int a){
	this->downColor = SDL_MapRGBA(SDL_GetVideoSurface()->format, r, g, b, a);
}

void SimpleButton::setButtonColorOver(int r,int g,int b,int a){
	this->overColor = SDL_MapRGBA(SDL_GetVideoSurface()->format, r, g, b, a);
}

void SimpleButton::onMouseDown(int x, int y, View* v){
	TextView::onMouseDown(x,y,v);
	if(inView(x,y))
		this->setBackgroundColor(this->downColor);
}

void SimpleButton::onMouseUp(int x, int y, View* v){
	TextView::onMouseUp(x,y,v);
	if(inView(x,y))
		this->setBackgroundColor(this->overColor);
}

void SimpleButton::onMouseMove(int x, int y, int xr, int yr, View * v){
	TextView::onMouseMove(x,y,xr,yr,v);
	if(inView(x,y))
		this->setBackgroundColor(this->overColor);
}

void SimpleButton::onMouseEnter(int x, int y, View * v){
	TextView::onMouseEnter(x,y,v);
	this->setBackgroundColor(this->overColor);
}

void SimpleButton::onMouseLeave(int x, int y, View * v){
	TextView::onMouseLeave(x,y,v);
	this->setBackgroundColor(this->normalColor);
}

void SimpleButton::draw(SDL_Surface * display, int delay){
	TextView::draw(display, delay);
}

SimpleButton::~SimpleButton(void)
{
}
