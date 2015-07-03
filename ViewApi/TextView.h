#pragma once
#include <iostream>
#include <SDL/SDL_ttf.h>
#include "View.h"

using namespace std;

class TextView :
	public virtual View
{
public:
	TextView(void);
	TextView(string);
	virtual void draw(SDL_Surface *, int);
	virtual void setText(string s);
	string getText();
	void setColor(Uint8 r, Uint8 g, Uint8 b);
	void setSize(int s);
	~TextView(void);
protected:
	TTF_Font * font;
	string text;
	int textSize;
	SDL_Color textColor;
	SDL_Surface *textSurface;
};

