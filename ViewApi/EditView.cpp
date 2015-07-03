#include "EditView.h"


EditView::EditView()
{
	focus=false;
	focusTime=0;
	this->setBackgroundColor(250,250,200,1);
	setPadding(0,3,0,0);
}

EditView::EditView(string s)
{	
	*this=EditView();
	setText(s);
}


void EditView::setFocus(bool f){
	focus = f;
	focusTime=0;
}


void EditView::draw(SDL_Surface * display, int delay){
	TextView::draw(display,delay);

	if(focus == false)
		return;
	focusTime += (delay+1);

	if(focusTime >= 1000){
		focusTime=0;
		return;
	}

	if(focusTime >= 500){
		return;
	}

	cursorPos.y=getY();
	cursorPos.w=1;
	cursorPos.h=this->getHeight();
	cursorPos.x=getX()+padLeft;

	if(textSurface != NULL){
		cursorPos.x += textSurface->w;
	}

	SDL_FillRect(display,&cursorPos,SDL_MapRGBA(SDL_GetVideoSurface()->format, 0, 0, 0, 1));
}

void EditView::onClick(int x, int y, View * v){
	TextView::onClick(x, y, v);
	setFocus(true);
}

void EditView::onMouseUp(int x, int y, View * v){
	TextView::onMouseUp(x, y, v);
	if(!inView(x,y)){
		setFocus(false);
	}
}

void EditView::onKeyUp(SDL_KeyboardEvent key, View * v) {
	TextView::onKeyUp(key, v);
	if(focus == false)
		return;
	if(key.keysym.sym == SDLK_BACKSPACE)
    {
		if(text.length() >0)
			text.erase(text.length() - 1);
    }else{
		text += key.keysym.sym;
	}

	setText(text);
}

EditView::~EditView(void)
{
}
