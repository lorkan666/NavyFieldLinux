#include "ImageView.h"
#include "AimingCursor.h"

#include "ImageView.h"

AimingCursor::AimingCursor(string path)
{
	aim_bar_img_path=path;
	width=70;
	height=70;
	//this->setBackgroundColor(0,40,40,1);
}

void AimingCursor::createBars(int count){
	for(int i=0;i<count;i++){
		ImageView * bar = new ImageView(aim_bar_img_path);
		this->addView(bar);
	}
}

ImageView * AimingCursor::getBar(int x){
	list<View*>::iterator it = childs.begin();
    for(int i=0; i<x; i++){
        ++it;
    }
    return (ImageView *)*it;
}
void AimingCursor::setPosition(int xx, int yy){
	x = xx-getWidth()/2;
	y = yy-getHeight()/2;
	//invalidateLayout();
}

AimingCursor::~AimingCursor(void)
{
}
