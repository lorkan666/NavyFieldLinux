#pragma once
#include "ImageView.h"
#include "RelativeLayout.h"

class AimingCursor :
	public RelativeLayout
{
public:
	AimingCursor(string path);
	~AimingCursor(void);
	void createBars(int count);
	void setPosition(int xx, int yy);
	ImageView * getBar(int x);
	string aim_bar_img_path;
private:
	
};

