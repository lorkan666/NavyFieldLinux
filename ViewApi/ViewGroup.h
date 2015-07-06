#ifndef view_group_h
#define view_group_h

#include "ViewParent.h"

class ViewGroup :public ViewParent
{
public:
	virtual void addView(View *);
	virtual void addView(View *,int pos);
	virtual void removeView(View *);
	virtual void removeViewAndDelete(View *);
	virtual void draw(SDL_Surface *, int);
	virtual void onMouseDown(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onMouseMove(int x, int y,int xr, int yr, View * v);
	virtual void onKeyDown(SDL_KeyboardEvent key , View * v);
	virtual void onKeyUp(SDL_KeyboardEvent key, View * v);
	virtual void onMouseWheelDown(int x, int y, View * v);
	virtual void onMouseWheelUp(int x, int y, View * v);
	View * findViewByName(string name);
	void setPosition(int x, int y);
	virtual void resize(int width,int height);
	virtual void setOffset(int offsetX, int offsetY);
	void updateLayout();
	ViewGroup(void);
	virtual ~ViewGroup(void);

};

#endif
