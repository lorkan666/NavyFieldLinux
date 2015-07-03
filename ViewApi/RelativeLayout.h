#pragma once
#include "ViewGroup.h"
class RelativeLayout :
	public ViewGroup
{
public:
	RelativeLayout(void);
	virtual void draw(SDL_Surface *, int);
	void updateLayout();
	~RelativeLayout(void);
};

