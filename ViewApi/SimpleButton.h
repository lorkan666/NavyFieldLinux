#pragma once
#include "TextView.h"
using namespace std;

class SimpleButton :
	public TextView
{
public:
	SimpleButton(string t);
	void setButtonColor(int r,int g,int b,int a);
	void setButtonColorDown(int r,int g,int b,int a);
	void setButtonColorOver(int r,int g,int b,int a);
	virtual void onMouseDown(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onMouseEnter(int x, int y, View * v) ;
	virtual void onMouseLeave(int x, int y, View * v) ;
	virtual void onMouseMove(int x, int y,int xr, int yr, View * v);
	virtual void draw(SDL_Surface * display, int delay);
	~SimpleButton(void);
protected:
	Uint32 normalColor;
	Uint32 downColor;
	Uint32 overColor;
};

