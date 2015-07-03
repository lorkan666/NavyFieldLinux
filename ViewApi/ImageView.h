#pragma once
#include <exception>
#include <SDL/SDL_rotozoom.h>
#include <iostream>
#include "View.h"

using namespace std;

class ImageView :public View
{
public:

	void setScale(double s);
	void setScale(double sx, double sy);
	void rotate(double r);
	void setRotation(double r);
	void setSmothing(bool s);
	double getRotation();
	virtual void resize(int width,int height);
	virtual void draw(SDL_Surface *, int);
	ImageView();
	ImageView(string path);
	~ImageView(void);

private:
	void transform();
	bool smothing;
	double rotation;
	double scaleX,scaleY,resizeX,resizeY;
	SDL_Surface * surface, *img;
};

