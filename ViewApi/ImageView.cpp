#include "ImageView.h"

#include <SDL/SDL_image.h>

ImageView::ImageView(){
	surface = NULL;
	img = NULL;
	rotation=0;
	scaleX=1.0;
	scaleY=1.0;
	resizeX = 1.0;
	resizeY = 1.0;
	smothing=true;
}

ImageView::ImageView(string path):ImageView()
{	
	SDL_Surface * tmp;
	tmp = IMG_Load(path.c_str());
	if(tmp == NULL){
		throw string("Error in: "+string(__PRETTY_FUNCTION__)+" details:"+SDL_GetError());
	}
	img = SDL_DisplayFormatAlpha(tmp);
	SDL_FreeSurface( tmp );
	if(img == NULL){
		throw string("Error in: "+string(__PRETTY_FUNCTION__)+" details:"+SDL_GetError());
	}
	height = img->h;
	width = img->w;
	surface=img;
}

void ImageView::setScale(double s){
	setScale(s, s);
}

void ImageView::setScale(double sx, double sy){
	scaleX=sx;
	scaleY=sy;
	transform();
}

void ImageView::resize(int w,int h){
	if(img == NULL)
		return;
	resizeX = (double)w/img->w;
	resizeY = (double)h/img->h;
	transform();
}

void ImageView::rotate(double r){
	rotation+=r;
	transform();
}

void ImageView::setRotation(double r){
	rotation=r;
	transform();
}

double ImageView::getRotation(){
	return rotation;
}


void ImageView::setSmothing(bool s) {
	smothing = s;
}

void ImageView::transform(){
	SDL_Surface * new_surface = rotozoomSurfaceXY(img,rotation,scaleX*resizeX,scaleY*resizeY,smothing);
	if(new_surface == NULL){
		throw string("Error in: "+string(__PRETTY_FUNCTION__)+" details:"+SDL_GetError());
	}
	if(surface != NULL && surface != img){
		SDL_FreeSurface( surface );
		surface = NULL;
	}

	surface = new_surface;
	View::resize(surface->w,surface->h);
}

void ImageView::draw(SDL_Surface * display, int delay){
#ifdef _DRAW
	        Uint32 startTicks = SDL_GetTicks();
#endif
			View::draw(display,delay);
			if(display == NULL ||  surface == NULL)
				return;
			SDL_Rect dst;
			dst.x=getX();
			dst.y=getY();
			dst.w=width;
			dst.h=height;

			SDL_BlitSurface(surface, NULL, display,&dst);
#ifdef _DRAW
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}

ImageView::~ImageView(void)
{
	if(surface != NULL && surface != img)
		SDL_FreeSurface( surface );

	if(img != NULL)
		SDL_FreeSurface( img );
}

