#include "GameMap.h"

#include <string>
#include <iostream>
#include <SDL/SDL_image.h>
#include <SDL/SDL_rotozoom.h>

#include "AimingCursor.h"
#include "Ship.h"

#define zoom_max  3
#define zoom_min  0.8

GameMap::GameMap(string path)
{
	surface = NULL;
	frame_surface = NULL;
	map_surface = NULL;
	img = NULL;
	frameToShip = false;
	moveFrameToTarget=false;
	invalidableByChilds(true);
	isMouseNearBorder = false;
	zoom_changed = false;
	movementThread=NULL;
	playerShip = NULL;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif

	frame_original.x=0;
	frame_original.y=0;
	frame_original.w=400;
	frame_original.h=300;
	frame.w = frame_original.w*zoom;
	frame.h = frame_original.h*zoom;
	frame.x=frame_original.x;
	frame.y=frame_original.y;
	zoom=1.0;
	if(!readProperties(path+"map.xml"))
		throw string("Cannot open:"+path+"map.xml");

	SDL_Surface * tmp;
	tmp = IMG_Load((path+fixPath(getStringProperty("map_img"))).c_str());
	if(tmp == NULL){
		throw string(SDL_GetError());
	}
	img = SDL_DisplayFormat(tmp);
	SDL_FreeSurface( tmp );
	if(img == NULL){
		throw string(SDL_GetError());
	}	
	
	AimingCursor * ac = new AimingCursor("");	
	setAimingCursor(ac);
	this->need_update_layout=1;
}

void GameMap::setPlayerShip(Ship * s){
	this->playerShip = s;
	aimingCursor->createBars(playerShip->getCannons().size());
}

void GameMap::resize(int width,int height){
	RelativeLayout::resize(width,height);
	recreateMapSurface();
	recreateFrameSurface();
	frame_original = frame;
	frame.w = getWidth()*zoom;
	frame.h = getHeight()*zoom;
}

void GameMap::repeatImgOnMapSurface(){
	if(map_surface == NULL ||  img == NULL)
		return;

	SDL_Rect dst;
	int xrepeatness = (map_surface->w/img->w)+1;
	int yrepeatness = (map_surface->h/img->h)+1;

	for(int i=0;i<xrepeatness;i++){
		for(int j=0;j<yrepeatness;j++){
			dst.x=i*img->w;
			dst.y=j*img->h;

			SDL_BlitSurface(img, NULL, map_surface,&dst);
		}
	}
}

void GameMap::recreateMapSurface(){
	if(map_surface != NULL)
		SDL_FreeSurface( map_surface );

	SDL_Surface * tmp;
	tmp = SDL_CreateRGBSurface(0, getWidth()*zoom_max + img->w, getHeight()*zoom_max + img->h, 32, rmask, gmask, bmask, amask);
	if(tmp == NULL){
		throw string(SDL_GetError());
	}
	map_surface = SDL_DisplayFormat(tmp);
	SDL_FreeSurface( tmp );
	if(map_surface == NULL){
		throw string(SDL_GetError());
	}
	repeatImgOnMapSurface();
	
}

void GameMap::recreateFrameSurface(){
	if(frame_surface != NULL)
		SDL_FreeSurface( frame_surface );

	SDL_Surface * tmp;
	tmp = SDL_CreateRGBSurface(0, getWidth()*zoom_max , getHeight()*zoom_max , 32, rmask, gmask, bmask, amask);
	if(tmp == NULL){
		throw string(SDL_GetError());
	}
	frame_surface = SDL_DisplayFormat(tmp);
	SDL_FreeSurface( tmp );
	if(frame_surface == NULL){
		throw string(SDL_GetError());
	}
}

void GameMap::updateFrameSurface(){	
	if( map_surface == NULL)
		recreateMapSurface();

	if( frame_surface == NULL)
		recreateFrameSurface();

	SDL_Rect frame_off;
	frame_off.x = - (frame.x % img->w);
	frame_off.y = - (frame.y % img->h);

	SDL_BlitSurface(map_surface, NULL, frame_surface,&frame_off);
}

void GameMap::updateLayout(){
#ifndef NDEBUG
	        Uint32 startTicks = SDL_GetTicks();
#endif
	frame_original = frame;
	frame.w = getWidth()*zoom;
	frame.h = getHeight()*zoom;
	fixFrame();
	fixAimingCursor();

	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		(*i)->setOffset(-frame.x,-frame.y);
	}
	
	updateFrameSurface();
	this->need_update_layout=0;
	RelativeLayout::draw(frame_surface,1);

	if(zoom != 1){
		SDL_Surface * tmp = rotozoomSurfaceXY(frame_surface,0,1/zoom,1/zoom,SMOOTHING_OFF);
		if(tmp == NULL){
			throw string(SDL_GetError());
		}
		if(surface != NULL)
			SDL_FreeSurface( surface );
		surface=tmp;
	}

#ifndef NDEBUG
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}

void GameMap::draw(SDL_Surface * display, int delay){
#ifdef _DRAW
	        Uint32 startTicks = SDL_GetTicks();
#endif
			if(this->need_update_layout == 1){
				this->updateLayout();
				this->need_update_layout=0;
			}
			if(display == NULL)
				return;

			SDL_Rect dst,src;

			src.x=0;
			src.y=0;
			src.w=getWidth();
			src.h=getHeight();

			dst.x=getX();
			dst.y=getY();
			if(zoom == 1){
				if(frame_surface == NULL)
					return;
				SDL_BlitSurface(frame_surface, &src, display,&dst);

			}else{
				if(surface == NULL){
					return;
				}
				SDL_BlitSurface(surface, &src, display,&dst);
			}
#ifdef _DRAW
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}


int GameMap::getMapWidth(){
	return this->getIntProperty("width");
}

int GameMap::getMapHeight(){
	return this->getIntProperty("height");
}

void GameMap::fixAimingCursor(){
	if(aimingCursor == NULL || playerShip == NULL)
		return;
	aimingCursor->setPosition(playerShip->aimPoint.x,playerShip->aimPoint.y);
	fixAimingCursorRotations();
	fixAimingCursorPosition();
}

void GameMap::fixAimingCursorRotations(){
	list<Cannon*> cannons = playerShip->getCannons();
	Cannon* c;
	int i=0;
	for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
		c = (Cannon *)*iter;
		aimingCursor->getBar(i)->setRotation(c->getRotation());
		i++;
	}
}

void GameMap::fixAimingCursorPosition(){
	list<Cannon*> cannons = playerShip->getCannons();
	double acx =aimingCursor->getWidth()/2;
	double acy =aimingCursor->getHeight()/2;
	Cannon* c;
	int i=0;
	if(playerShip->getAimingToPoint()){
		for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
			c = (Cannon *)*iter;
			aimingCursor->getBar(i)->x = acx-aimingCursor->getBar(i)->getWidth()/2;
			aimingCursor->getBar(i)->y = acy-aimingCursor->getBar(i)->getHeight()/2;
			i++;
		}
		return;
	}

	for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
		c = (Cannon *)*iter;
		aimingCursor->getBar(i)->x = acx-aimingCursor->getBar(i)->getWidth()/2+(playerShip->x + c->x+c->getWidth()/2 - playerShip->centerX);
		aimingCursor->getBar(i)->y = acy-aimingCursor->getBar(i)->getHeight()/2+(playerShip->y + c->y+c->getHeight()/2 - playerShip->centerY);
		i++;
	}
}

bool GameMap::isMouseCloseBorder(){		
	bool result = false;
	if(lastMouseX < getX()+mouseBorderOffset && frame.x>0){
		result=true;
	}
	if(lastMouseX > getX()+getWidth()-mouseBorderOffset && frame.x<getMapWidth()-frame.w){
		result=true;
	}
	if(lastMouseY < getY()+mouseBorderOffset && frame.y>0){
		result=true;
	}
	if(lastMouseY > getY()+getHeight()-mouseBorderOffset && frame.y<getMapHeight()-frame.h){
		result=true;
	}
	return result;
}

void GameMap::fixFrame(){
	if(frame.x<0){
		frame.x=0;
	}
	if(frame.x>getMapWidth() - frame.w){
		frame.x=getMapWidth() - frame.w;
	}
	if(frame.y<0){
		frame.y=0;
	}
	if(frame.y>getMapHeight() - frame.h){
		frame.y = getMapHeight() - frame.h;
	}
	int xr = lastMouseX;
	int yr = lastMouseY;
	setToInMapMouseXY(&xr,&yr);
	playerShip->aimTo(xr,yr);
}

void GameMap::setAimBarPath(string path){
	this->aim_bar_img_path = path;
	aimingCursor->aim_bar_img_path = path;
}

void GameMap::setAimingCursor(AimingCursor * a){
	//if(a == NULL)
	//	return;
	//if(aimingCursor!=NULL)
	//	delete aimingCursor;
	aimingCursor = a;
	this->addView(aimingCursor);
}

int GameMap::moveFrame(void* data){
	GameMap * gm = (GameMap *)data;
	double adder = 2;
	while(gm!=NULL && (gm->isMouseNearBorder || gm->moveFrameToTarget)){
		//cout<<"test"<<endl;
		cout<< gm->isMouseNearBorder<<"," <<gm->moveFrameToTarget <<endl;
		if(gm->moveFrameToTarget){
			if(gm->frameToShip){
				gm->frame.x = gm->playerShip->centerX-gm->frame.w/2;
				gm->frame.y = gm->playerShip->centerY-gm->frame.h/2;
				gm->moveFrameToTarget=false;
			}else{
				if(gm->playerShip->lastShoot.x == (-1)){
					gm->moveFrameToTarget=false;
					continue;
				}
				gm->frame.x = gm->playerShip->lastShoot.x-gm->frame.w/2;
				gm->frame.y = gm->playerShip->lastShoot.y-gm->frame.h/2;
				gm->moveFrameToTarget=false;
			}
		}else{
			if(gm!=NULL && gm->lastMouseX < gm->getX()+mouseBorderOffset && gm->frame.x>0){
				gm->frame.x -= adder;
			}
			if(gm!=NULL && gm->lastMouseX > gm->getX()+gm->getWidth()-mouseBorderOffset && gm->frame.x<gm->getMapWidth()-gm->frame.w){
				gm->frame.x += adder;
			}
			if(gm!=NULL && gm->lastMouseY < gm->getY()+mouseBorderOffset && gm->frame.y>0){
				gm->frame.y -= adder;
			}
			if(gm!=NULL && gm->lastMouseY > gm->getY()+gm->getHeight()-mouseBorderOffset && gm->frame.y<gm->getMapHeight()-gm->frame.h){
				gm->frame.y += adder;
			}	
			if(adder<10)
				adder*=1.015;
			SDL_Delay(50/adder);
		}
		gm->need_update_layout = 1;
		
	}
	gm->movementThread = NULL;
	return 0;
}

void GameMap::switchFramePos(){
	frameToShip = !frameToShip;
	moveFrameToTarget = true;
	if(movementThread==NULL)
		movementThread = SDL_CreateThread(&GameMap::moveFrame,this);
}

void GameMap::setToInMapMouseXY(int *xx, int *yy){
	double xa = frame.x +  1.0*(*xx-getX())*zoom;
	double ya = frame.y +  1.0*(*yy-getY())*zoom;
	
	*xx=xa;
	*yy=ya;
}

void GameMap::onMouseDown(int x, int y, View* p){
	RelativeLayout::onMouseDown( x, y, this);
	playerShip->shoot();
}

void GameMap::onMouseUp(int x, int y, View* p){
	RelativeLayout::onMouseUp(x,y,this);
	
}

void GameMap::onMouseEnter(int x, int y, View * v){
	RelativeLayout::onMouseEnter(x, y,this);
	SDL_ShowCursor(0);
}

void GameMap::onMouseLeave(int x, int y, View * v){
	RelativeLayout::onMouseLeave(x, y,this);
	SDL_ShowCursor(1);
}

void GameMap::onMouseMove(int x, int y, int xr, int yr, View * v){
	xr=x;
	yr=y;
	setToInMapMouseXY(&xr,&yr);
	RelativeLayout::onMouseMove(x, y, xr, yr, this);
	if(!inView(x,y))
		return;

	lastMouseX = x;
	lastMouseY = y;
	isMouseNearBorder = isMouseCloseBorder();
	if(isMouseNearBorder){
		moveFrameToTarget = false;
		frameToShip = false;
	}

	if(isMouseNearBorder==true && movementThread==NULL)
		movementThread = SDL_CreateThread(&GameMap::moveFrame,this);
		
	playerShip->aimTo(xr,yr);
}

void GameMap::zoomTo(int xx, int yy){
	frame_original = frame;
	frame.w = getWidth()*zoom;
	frame.h = getHeight()*zoom;
	double offX = (xx-getX())*1.0/getWidth();
	double offY = (yy-getY())*1.0/getHeight();
	double difX = (frame_original.w-frame.w)*offX;
	double difY = (frame_original.h-frame.h)*offY;
	frame.x = frame_original.x+difX;
	frame.y = frame_original.y+difY;
	fixFrame();
	zoom_changed=true;
	moveFrameToTarget = false;
	frameToShip = false;
	setToInMapMouseXY(&xx,&yy);
	playerShip->aimTo(xx,yy);
}

void GameMap::onMouseWheelDown(int x, int y, View * v){
	RelativeLayout::onMouseWheelDown( x,  y, v);
	if(!inView(x,y))
		return;
	if(zoom>=zoom_max)
		return;

	zoom+=0.04;
	zoomTo(x, y);
	this->need_update_layout=1;
}
void GameMap::onMouseWheelUp(int x, int y, View * v){
	RelativeLayout::onMouseWheelUp( x,  y, v);
	if(!inView(x,y))
		return;
	if(zoom<=zoom_min)
		return;
	
	zoom-=0.04;	
	zoomTo(x, y);
	this->need_update_layout=1;
}

GameMap::~GameMap(void)
{
	isMouseNearBorder=false;
	try{
		SDL_KillThread(movementThread);
	}catch(...){}

	if(surface != NULL)
		SDL_FreeSurface( surface );
	if(map_surface != NULL)
		SDL_FreeSurface( map_surface );
	if(img != NULL)
		SDL_FreeSurface( img );
	if(frame_surface != NULL && surface != frame_surface)
		SDL_FreeSurface( frame_surface );
}

bool GameMap::getIsMouseNearBorder() const {
	return isMouseNearBorder;
}

bool GameMap::isMoveFrameToTarget() const {
	return moveFrameToTarget;
}
