#include "PasswordView.h"

#include <string>


PasswordView::PasswordView(void)
{
}

void PasswordView::setText(string s){
	string stars(s.length(), '*');
	EditView::setText(stars);
	text=s;
}

void PasswordView::draw(SDL_Surface * display, int delay){
	EditView::draw(display,delay);
}

PasswordView::~PasswordView(void)
{
}
