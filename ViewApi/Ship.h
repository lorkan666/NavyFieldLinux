#pragma once
#include <iostream>

#include "ImageView.h"
#include "PropertiesConfigReader.h"
#include "RelativeLayout.h"
#include "Cannon.h"
#include "Salvo.h"

class Salvo;
class Ship :
	public RelativeLayout,
	public PropertiesConfigReader
{

	friend class Salvo;

public:
	typedef struct {
	   int x;
	   int y;
	   int w;
	   int h;
	 } My_Rect;

	double speed;
	double acceleration;
	void draw(SDL_Surface * display, int delay);
	Ship(string s);
	Ship(void);
	void rotate(double r);
	void setRotation(double r);
	void aimTo(int x, int y);
	void updateLayout();
	void startMove();
	void switchAiming();
	bool getAimingToPoint();
	list<Cannon*> getCannons();
	virtual void setPosition(int x, int y);
	double aimDistance;
	My_Rect aimPoint;
	void shoot();
	double centerX, centerY;
	void speedUp();
	void speedDown();
	void turnLeft();
	void turnRight();
	My_Rect lastShoot;
	~Ship(void);

private:
	Salvo * lastSalvo;
	bool aimingToPoint;
	Uint32 lastShootTicks;
	SDL_Thread * movementThread;
	static int move(void* data);
	string shipPath;
	ImageView * ship_img;
	list<Cannon*> cannons;
	void fixPosition();
	void fixCannonsPosition();
	void fixCannonsRotation();
	void addCannons(string img,string cfg);
};

