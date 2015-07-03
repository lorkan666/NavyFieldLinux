#pragma once
#include "EditView.h"

class PasswordView :
	public EditView
{
public:
	PasswordView(void);
	virtual void draw(SDL_Surface * display, int delay);
	void setText(string s);
	~PasswordView(void);
};

