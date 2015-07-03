#pragma once
#include "ImageView.h"
#include "Ship.h"

class Shell :
	public ImageView
{
public:
	typedef struct {
		   int x;
		   int y;
		   int w;
		   int h;
		 } My_Rect;
	My_Rect begin,target;
	double currentDistance,targetDistance;
	Shell(string path);
	~Shell(void);
};

