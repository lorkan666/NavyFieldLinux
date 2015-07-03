#include "GameMap.h"
#include "PlayerControler.h"

#include "GameMap.h"
#include "Ship.h"


PlayerControler::PlayerControler(GameMap * gm)
{
	this->gm = gm;
	this->player=gm->playerShip;
}

void PlayerControler::onClick(int x, int y, View * v){
}

void PlayerControler::onMouseDown(int x, int y, View * v){
}

void PlayerControler::onMouseUp(int x, int y, View * v){

}

void PlayerControler::onMouseMove(int x, int y, int xr, int yr, View * v){
	
}

void PlayerControler::onMouseEnter(int x, int y, View * v){

}

void PlayerControler::onMouseLeave(int x, int y, View * v){
}

void PlayerControler::onKeyDown(SDL_KeyboardEvent key , View * v){
	if(player == NULL)
		return;

	switch(key.keysym.sym )
    {
		case SDLK_a:
			player->turnLeft();
		break;

		case SDLK_d:
			player->turnRight();
		break;

		case SDLK_w:
			player->speedUp();
		break;

		case SDLK_s:
			player->speedDown();
		break;

		case SDLK_e:
			player->switchAiming();
		break;

		case SDLK_SPACE:
			player->shoot();
		break;

		case SDLK_g:
			gm->switchFramePos();
		break;
		default:
		break;
    }
}

void PlayerControler::onMouseWheelDown(int x, int y, View * v){

}
void PlayerControler::onMouseWheelUp(int x, int y, View * v){

}

void PlayerControler::onKeyUp(SDL_KeyboardEvent key, View * v){

}


PlayerControler::~PlayerControler(void)
{
}
