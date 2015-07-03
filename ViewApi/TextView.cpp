#include "TextView.h"


TextView::TextView(void)
{
	textSurface=NULL;
	text = "";
	textSize = 18;
	if (TTF_Init() != 0){
		throw string(TTF_GetError());
	}
	font = TTF_OpenFont("FreeSans.ttf", textSize);
	if (font == NULL){
		TTF_Quit();
		throw string(TTF_GetError());
	}

	int wtmp,htmp;
	if(TTF_SizeText(font,"He",&wtmp,&htmp)) {
	}

	textColor.r = 0;
	textColor.g = 0;
	textColor.b = 0;
	
	height = htmp;
}

TextView::TextView(string t):TextView(){
	setText(t);
}

void TextView::setColor(Uint8 r, Uint8 g, Uint8 b){
	textColor.r = r;
	textColor.g = g;
	textColor.b = b;
	setText(text);
}

void TextView::setSize(int s){
	textSize = s;
	font = TTF_OpenFont("FreeSans.ttf", textSize);
	if (font == NULL){
		TTF_Quit();
		throw string(TTF_GetError());
	}
	int wtmp,htmp;
	if(TTF_SizeText(font,"He",&wtmp,&htmp)) {
	}
	height = htmp;
	setText(text);
}

string TextView::getText(){
	return text;
}

void TextView::setText(string s){

	if(s.empty()){
		if(textSurface != NULL)
			SDL_FreeSurface( textSurface );
		textSurface = NULL;
		return;
	}
	text = s;
	SDL_Surface * newTextSurface;
	newTextSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);

	if(newTextSurface == NULL)
	{
      cerr << "TTF_RenderText_Blended() Failed: " << TTF_GetError() << endl;
      TTF_Quit();
	  return;
	}
	if(textSurface != NULL)
		SDL_FreeSurface( textSurface );
	textSurface = newTextSurface;
	resize(textSurface->w,textSurface->h);
}

void TextView::draw(SDL_Surface * display, int delay){
	View::draw(display,delay);
	if(display == NULL ||  textSurface == NULL)
		return;

	SDL_Rect dst;
	dst.x=getX()+padLeft;
	dst.y=getY()+padTop;
	dst.w=getWidth();
	dst.h=getHeight();

	SDL_BlitSurface(textSurface, NULL, display,&dst);	
}

TextView::~TextView(void)
{
	if(textSurface != NULL)
		SDL_FreeSurface( textSurface );
}
