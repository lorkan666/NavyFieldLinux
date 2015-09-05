#include "Ship.h"

#include "Markup.h"
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>

#include "Cannon.h"
#include "Markup.h"
#include "Salvo.h"
#include "Shell.h"

Ship::Ship(){
	acceleration = 1;
	aimingToPoint = true;
	centerX=0;
	centerY=0;
	aimDistance=0.0;
	ship_img=NULL;
	speed=0;
	aimPoint.x=0;
	aimPoint.y=0;
	movementThread=NULL;
	shipPath = "";
	invalidableByChilds(false);
	lastSalvo=NULL;
	lastShootTicks = 0;
	lastShoot.x = -1;
}

Ship::Ship(string path):Ship()
{
	RelativeLayout();
	shipPath=path;
	if(!readProperties(path+"ship.xml"))
		throw string("Cannot open:"+path+"ship.xml");

	ship_img = new ImageView(path+getStringProperty("ship_img"));
	if(ship_img == NULL){
		throw string("Cannot open: "+path+getStringProperty("ship_img"));
	}
	this->addView(ship_img);

	addCannons(path+getStringProperty("cannon_img"),path+getStringProperty("cannon_cfg"));
	this->need_update_layout=1;
}

void Ship::addCannons(string img_path,string config_path){
	CMarkup xml;

	if(!xml.Load( config_path )){
		throw string(xml.GetError());
	}
	xml.FindElem();
	xml.IntoElem();


	while(xml.FindElem()){
		string data="";
		Cannon * c = new Cannon(img_path);

		xml.IntoElem();
		xml.FindElem("posY");
		data.append(xml.GetData()+" ");
		xml.FindElem("posX");
		data.append(xml.GetData()+" ");
		xml.FindElem("orientation");
		data.append(xml.GetData()+" ");
		xml.FindElem("shells_count");
		data.append(xml.GetData()+" ");

		istringstream iss(data);

		iss >> c->posY;
		iss >> c->posX;
		iss >> c->orientation;
		iss >> c->shells_count;

		if(c->orientation == 0)
			c->setRotation(180);

		cannons.push_back(c);
		this->addView(c);

		xml.OutOfElem();
	}


}

list<Cannon*> Ship::getCannons(){
	return cannons;
}

void Ship::shoot(){
	Uint32 now = SDL_GetTicks();
	if(lastShootTicks!=0 && (now-lastShootTicks) < getDoubleProperty("reload")*1000 ){
		return;
	}
	lastShoot.x = aimPoint.x;
	lastShoot.y = aimPoint.y;

	lastShootTicks=now;
	Salvo * salvo = new Salvo((Ship*)this);
	getParent()->addView(salvo);
	salvo->startMove();
}

void Ship::fixPosition(){
	x = centerX-getWidth()/2;
	y = centerY-getHeight()/2;
	RelativeLayout::updateLayout();
	if(parent != NULL)
			parent->invalidateLayout();
}

void Ship::switchAiming(){
	aimingToPoint=!aimingToPoint;
	fixCannonsRotation();
	if(parent != NULL)
			parent->invalidateLayout();
}

bool Ship::getAimingToPoint(){
	return aimingToPoint;
}

void Ship::setPosition(int xx, int yy){
	centerX = xx;
	centerY = yy;
	fixPosition();
}

void Ship::updateLayout(){
#ifdef _UPDATE
	        Uint32 startTicks = SDL_GetTicks();
#endif
	RelativeLayout::updateLayout();
	resize(ship_img->getWidth(), ship_img->getHeight());
	fixPosition();
	fixCannonsRotation();
	fixCannonsPosition();
	
#ifdef _UPDATE
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
	
}

void Ship::aimTo(int xc, int yc){
	need_update_layout=1;
	if(parent != NULL)
		parent->invalidateLayout();

	double range = getDoubleProperty("range")*1000;
	aimDistance = View::getDistance(centerX,centerY,xc,yc);

	aimPoint.w = xc-centerX;
	aimPoint.h = yc-centerY;

	if(aimDistance <= range){
		aimPoint.x=xc;
		aimPoint.y=yc;
		return;
	}
	aimPoint.w = aimPoint.w*range/aimDistance;
	aimPoint.h = aimPoint.h*range/aimDistance;
	aimDistance = range;

	aimPoint.x = centerX + aimPoint.w;
	aimPoint.y = centerY + aimPoint.h;
}

void Ship::fixCannonsRotation(){
	if(parent != NULL)
			parent->invalidateLayout();
	Cannon * c;
	double angle;

	if(aimingToPoint){
		double xCenter,yCenter;		
		for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
			c = *iter;
			xCenter=this->x + c->x+c->getWidth()/2;
			yCenter=this->y + c->y+c->getHeight()/2;
			if(xCenter==aimPoint.x)
				return;
			angle = View::getAngle(xCenter,yCenter,aimPoint.x,aimPoint.y);
		
			c->setRotation(angle);
		}		
		return;
	}

	angle = View::getAngle(centerX,centerY,aimPoint.x,aimPoint.y);
	for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
		c = *iter;
		c->setRotation(angle);
	}	
	need_update_layout=1;
}

void Ship::fixCannonsPosition(){
	Cannon * c;
	double newX,newY;
	double a = ship_img->getRotation();
	for( std::list<Cannon *>::iterator iter = cannons.begin(); iter != cannons.end(); ++iter ){
		c = *iter;
		newX=c->posX*cos(-M_PI*a/180)+c->posY*sin(-M_PI*a/180);
		newY=c->posX*sin(-M_PI*a/180)+c->posY*cos(-M_PI*a/180);
		c->setPosition(newX+(getWidth()-c->getWidth())/2,newY+(getHeight()-c->getHeight())/2);
	}
	need_update_layout=1;
	if(parent != NULL)
			parent->invalidateLayout();
}

void Ship::draw(SDL_Surface * display, int delay){
	#ifdef _DRAW
		Uint32 startTicks = SDL_GetTicks();
	#endif
	RelativeLayout::draw(display,delay);
	#ifdef _DRAW
		cout<<__FUNCTION__<<" - " <<SDL_GetTicks() - startTicks<<endl;
	#endif
}

void Ship::startMove(){
	if(movementThread!=NULL)
		return;

	movementThread = SDL_CreateThread(&Ship::move,this);
}

int Ship::move(void* data){
	Ship * s = (Ship *)data;
	while(s !=NULL && s->speed!=0){
		double distance = s->speed*0.5144/20;
		double direction = -s->ship_img->getRotation()*M_PI/180;
		s->centerX+=(distance)*cos(direction);
		s->centerY+=(distance)*sin(direction);
		if(s->parent != NULL)
			s->parent->invalidateLayout();
		SDL_Delay(50);
	}
	s->movementThread = NULL;
	return 0;
}

void Ship::speedUp(){
	if(speed < getDoubleProperty("max_speed"))
		speed+=getDoubleProperty("acceleration");
	startMove();
}

void Ship::speedDown(){
	if(speed > getDoubleProperty("min_speed"))
		speed-=getDoubleProperty("acceleration");
	startMove();
}

void Ship::turnLeft(){
	if(speed != 0)
		rotate(+getDoubleProperty("turning_speed") * speed);
}

void Ship::turnRight(){
	if(speed != 0)
		rotate(-getDoubleProperty("turning_speed") * speed);
}

void Ship::rotate(double r){
	ship_img->rotate(r);
	updateLayout();
	if(parent != NULL)
			parent->invalidateLayout();
}

void Ship::setRotation(double r){
	ship_img->setRotation(r);
	updateLayout();
	if(parent != NULL)
			parent->invalidateLayout();
}

Ship::~Ship(void){
	speed=0;
	try{
		SDL_KillThread(movementThread);
	}catch(...){}
}
