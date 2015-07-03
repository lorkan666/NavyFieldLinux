#include "Salvo.h"

#include <string>
#include <ctime>
#include <cmath>

#include "GameMap.h"
#include "Ship.h"

Salvo::Salvo(){
	stillFly=false;
	ship=NULL;
	movementThread=NULL;
}

Salvo::Salvo(Ship * sship):Salvo()
{
	ship = sship;
	string shell_img = ship->shipPath + ship->getStringProperty("shell_img");

	int spread = ship->getIntProperty("spread");

	Cannon * c;

	srand( time( NULL ) );
	int sspan = ship->getIntProperty("shells_initial_span");
	int totalSpan = sspan * ship->getIntProperty("shells_per_cannon");
	if(ship->aimingToPoint){
		for( std::list<Cannon *>::iterator iter = ship->cannons.begin(); iter != ship->cannons.end(); ++iter ){
			c = (Cannon *)*iter;
			for(int j=0;j<c->shells_count;j++){
				Shell * s = new Shell(shell_img);
				s->begin.x = ship->x+c->x + (c->getWidth()-s->getWidth())/2 - (-totalSpan/2 + j*sspan)*sin((180-c->getRotation())*M_PI/180);
				s->begin.y = ship->y+c->y + (c->getHeight()-s->getHeight())/2 + (-totalSpan/2 + j*sspan)*cos((180-c->getRotation())*M_PI/180);
				s->target.x = ship->aimPoint.x-spread/2+rand()%spread;
				s->target.y = ship->aimPoint.y-spread/2+rand()%spread;
				s->x = s->begin.x;
				s->y = s->begin.y;
				s->begin.w = s->target.x-s->begin.x;
				s->begin.h = s->target.y-s->begin.y;
				s->targetDistance = View::getDistance(s->begin.x,s->begin.y,s->target.x,s->target.y);
				this->addView(s);			
			}
		}
		return;
	}
	for( std::list<Cannon *>::iterator iter = ship->cannons.begin(); iter != ship->cannons.end(); ++iter ){
		c = (Cannon *)*iter;
			for(int j=0;j<c->shells_count;j++){
				Shell * s = new Shell(shell_img);
				s->begin.x = ship->x+c->x + (c->getWidth()-s->getWidth())/2 - (-totalSpan/2 + j*sspan)*sin((180-c->getRotation())*M_PI/180);
				s->begin.y = ship->y+c->y + (c->getHeight()-s->getHeight())/2 + (-totalSpan/2 + j*sspan)*cos((180-c->getRotation())*M_PI/180);

				s->target.x = s->begin.x + ship->aimPoint.w-spread/2+rand()%spread;
				s->target.y = s->begin.y + ship->aimPoint.h-spread/2+rand()%spread;

				s->x = s->begin.x;
				s->y = s->begin.y;
				s->begin.w = s->target.x-s->begin.x;
				s->begin.h = s->target.y-s->begin.y;
				s->targetDistance = View::getDistance(s->begin.x,s->begin.y,s->target.x,s->target.y);
				this->addView(s);			
			}
		}
}

void Salvo::setOffset(int offsetX, int offsetY){
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		(*i)->setOffset(offsetX,offsetY);
	}
	this->need_update_layout=1;
}

void Salvo::startMove(){
	if(movementThread!=NULL)
		return;
	stillFly = true;
	movementThread = SDL_CreateThread(&Salvo::move,this);
}

int Salvo::move(void* data){
	Salvo * sal = (Salvo *)data;
	Shell * s;
	//GameMap * map = (GameMap *)sal->ship->getParent();
	double velocity = sal->ship->getDoubleProperty("shells_velocity");
	while(sal !=NULL && sal->stillFly){
		sal->stillFly=false;
		for( std::list<View *>::iterator iter = sal->childs.begin(); iter != sal->childs.end(); ++iter )
		{
			s = (Shell *)*iter;
			if((s->currentDistance + velocity/20 ) < s->targetDistance){
				s->currentDistance += velocity/20;
				s->x = s->begin.x + s->begin.w*s->currentDistance/s->targetDistance;
				s->y = s->begin.y + s->begin.h*s->currentDistance/s->targetDistance;
				sal->stillFly=true;
			}else{
				s->x = s->target.x;
				s->y = s->target.y;
			}
		}
		if(sal->parent != NULL)
			sal->parent->invalidateLayout();
		SDL_Delay(50);
	}
	sal->movementThread = NULL;
	//map->removeViewAndDelete(sal);
	//delete sal;
	return 0;
}

void Salvo::draw(SDL_Surface * display, int delay){
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		(*i)->draw(display, delay);
	}
}


Salvo::~Salvo(void)
{
	stillFly=0;
	try{
		if(movementThread!=NULL)
			SDL_KillThread(movementThread);
	}catch(...){}
}
