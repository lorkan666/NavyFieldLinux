#pragma once
#include "TextView.h"

class EditView :
	public TextView
{
public:
	EditView();
	EditView(string s);
	void setFocus(bool f);
	virtual void draw(SDL_Surface * display, int delay);
	virtual void onClick(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onKeyUp(SDL_KeyboardEvent key , View * v);
	~EditView(void);
protected:
	bool focus;
	int focusTime;
	SDL_Rect cursorPos;
};

