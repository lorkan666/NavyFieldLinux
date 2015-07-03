#include "View.h"

#include "typeinfo"
#include <iostream>
#include <cmath>

#include "RelativeLayout.h"
#include "ViewGroup.h"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using namespace std;

View::View()
{
	x=0;
	y=0;
	height = 0;
	width = 0;
	offsetX=0;
	offsetY=0;
	weight=1;
	backgroundColor = -1;
	fillParentHorizontal = false;
	fillParentVertical = false;
	onMouseEventListener = NULL;
	onKeyboardEventListener = NULL;
	onKeyDownCallback = NULL;
	onKeyUpCallback = NULL;
	this->onMouseClickCallback = NULL;
	parent = NULL;
	wasMouseDown = false;
	wasInView = false;
	setPadding(0,0,0,0);
}

View::View(int w, int h):View()
{
	width = w;
	height = h;
}

void View::setBackgroundColor(int r,int g,int b,int a){
	this->backgroundColor = SDL_MapRGBA(SDL_GetVideoSurface()->format, r, g, b, a);
}

void View::setBackgroundColor(Uint32 c){
	this->backgroundColor = c;
}

double View::getAngle(double fromX,double fromY,double toX,double toY){
	if(fromX==toX){
		if(fromY>toY)
			return 90;
		else
			return 270;
	}
		
	return 180-atan2(fromY-toY ,fromX-toX)* 180 / M_PI;
}

double View::getDistance(double fromX,double fromY,double toX,double toY){
	if(fromX==toX || fromY==toY)
		return 0;
	return sqrt((fromX-toX)*(fromX-toX)+(fromY-toY)*(fromY-toY));
}

int View::getWidth(){	
	return this->width + padLeft + padRight;
}

int View::getHeight(){
	return this->height + padTop + padBottom;
}

int View::getX(){	
	return this->x + offsetX;
}

int View::getY(){
	return this->y + offsetY;
}

void View::setOffset(int offsetX, int offsetY){
	this->offsetX = offsetX;
	this->offsetY = offsetY;
}

void View::setName(string name){
	this->name = name;
}

void View::setWeight(int w){
	this->weight = w;
	if(parent != NULL)
		parent->invalidateLayout();
}
int View::getWeight(){
	return this->weight;
}

void View::setFillParent(bool hortizonal, bool vertical){
	fillParentHorizontal = hortizonal;
	fillParentVertical = vertical;
}

bool View::getFillParentHortizonal(){
	return fillParentHorizontal;
}

bool View::getFillParentVertical(){
	return fillParentVertical;
}

void View::setPadding(int padTop, int padLeft, int padBottom, int padRight){
	this->padTop = padTop;
	this->padLeft = padLeft;
	this->padBottom = padBottom;
	this->padRight = padRight;
}

void View::setOnMouseClickCallback(OnMouseClickCallback callback){
	this->onMouseClickCallback = callback;
}

void View::setOnKeDownCallback(OnKeyCallback callback){
	this->onKeyDownCallback = callback;
}

void View::onMouseDown(int x, int y, View* v){
	if(!inView(x,y))
		return;
	wasMouseDown = true;
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseDown(x, y, this);
}

void View::onClick(int x, int y, View* v){
	if(!inView(x,y))
		return;
	if(onMouseEventListener != NULL)
		onMouseEventListener->onClick(x, y, this);
	if(onMouseClickCallback != NULL)
		onMouseClickCallback(x,y,this);
}

void View::onMouseUp(int x, int y, View* v){
	if(inView(x,y)){
		if(onMouseEventListener != NULL)
			onMouseEventListener->onMouseUp(x, y, this);

		if(wasMouseDown){
			this->onClick(x, y, v);
		}
	}
	wasMouseDown = false;
}

void View::setOnMouseEventListener(OnMouseEventListener * onMouseEventListener){
	this->onMouseEventListener = onMouseEventListener;
}

void View::setOnKeyboardEventListener(OnKeyboardEventListener * onKeyboardEventListener){
	this->onKeyboardEventListener = onKeyboardEventListener;
}

void View::onMouseMove(int x, int y, int xr, int yr, View * v){
	if(!inView(x,y)){
		if(wasInView){
			this->onMouseLeave(x,y,this);
		}
		wasInView = false;
		if(onMouseEventListener != NULL)
			onMouseEventListener->onMouseMove(x, y, xr,yr,this);
		return;
	}
	if(!wasInView){
			this->onMouseEnter(x,y,this);
	}
	wasInView = true;
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseMove(x, y, xr,yr,this);
}

void View::onMouseEnter(int x, int y, View * v){
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseEnter(x, y,this);
}

void View::onMouseLeave(int x, int y, View * v){
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseLeave(x, y,this);
}

void View::onMouseWheelDown(int x, int y, View * v){
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseWheelDown(x, y,this);
}
void View::onMouseWheelUp(int x, int y, View * v){
	if(onMouseEventListener != NULL)
		onMouseEventListener->onMouseWheelUp(x, y,this);
}

void View::onKeyDown(SDL_KeyboardEvent key , View * v) {
	if(onKeyboardEventListener != NULL)
		onKeyboardEventListener->onKeyDown(key,v);
	if(onKeyDownCallback != NULL)
		onKeyDownCallback(key,this);
}

void View::onKeyUp(SDL_KeyboardEvent key, View * v) {
	if(onKeyboardEventListener != NULL)
		onKeyboardEventListener->onKeyUp(key,v);
	if(onKeyUpCallback != NULL)
		onKeyUpCallback(key,this);
}

void View::setPosition(int x, int y){
	this->x = x;
	this->y = y;
}

bool View::inView(int xt, int yt){
	ViewGroup * p = getParent();

	if (p != NULL &&  typeid(*p) == typeid(RelativeLayout)) {
		if((xt<(x+p->x))||(xt>(x+p->x)+getWidth())||(yt<(y+p->y))||(yt>(y+p->y)+getHeight())){
			return false;
		}
		return true;
	}

	if((xt<x)||(xt>x+getWidth())||(yt<y)||(yt>y+getHeight())){
		return false;
	}
	return true;
}

void View::setHeight(int h){
	this->resize(width,h);
}

void View::setWidth(int w){
	this->resize(w,height);
}

void View::resize(int width,int height){
	this->width = width;
	this->height = height;
	if(parent != NULL)
		parent->invalidateLayout();
}

ViewGroup* View::getParent(){
	return this->parent;
}

void View::draw(SDL_Surface * display, int delay){
	#ifdef _DRAW
		Uint32 startTicks = SDL_GetTicks();
	#endif
	if(display == NULL)
			return;
	if(backgroundColor == (-1))
		return;

	SDL_Rect dst;
	dst.x=getX();
	dst.y=getY();
	dst.w=getWidth();
	dst.h=getHeight();
	SDL_FillRect(display,&dst,backgroundColor);
	#ifdef _DRAW
		cout<<__FUNCTION__<<" - " <<SDL_GetTicks() - startTicks<<endl;
	#endif
}

View::~View(void)
{
	if(onMouseEventListener != NULL)
		delete onMouseEventListener;
	onMouseEventListener = NULL;
}

