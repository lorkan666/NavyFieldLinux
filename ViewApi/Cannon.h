#ifndef cannon_h
#define cannon_h

#include <iostream>
#include "ImageView.h"

using namespace std;

class Cannon :
	public ImageView
{
	friend class Salvo;
public:
	Cannon(string img_path);
	~Cannon(void);
	int posX;
	int posY;
	int shells_count;
	int orientation;
};

#endif

