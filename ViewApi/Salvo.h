#pragma once
#include "RelativeLayout.h"
#include "Shell.h"


class Ship;
class Salvo :
	public RelativeLayout
{
public:
	Salvo(void);
	Salvo(Ship * s);
	virtual void draw(SDL_Surface *, int);
	virtual void setOffset(int x, int y);
	void startMove();
	~Salvo(void);

private:
	SDL_Thread * movementThread;
	bool stillFly;
	static int move(void* data);
	Ship * ship;
};

