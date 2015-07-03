#include "LinearLayout.h"

#include <iostream>
using namespace std;

LinearLayout::LinearLayout()
{
	orientation = HORIZONTAL;
	spacing = 0;
	invalidableByChilds(true);
	alignment = ALIGN_CENTER_VERTICAL | ALIGN_CENTER_HORIZONTAL;
	need_update_layout = 1;
}

void LinearLayout::updateLayout(){
#ifndef NDEBUG
	        Uint32 startTicks = SDL_GetTicks();
#endif
			//cout<<"LinearLayout::updateLayout"<<endl;
			int weightSum = getChildsWeight();
			unsigned int totalSpacing = (childs.size()-1)*spacing;
			list<View*>::iterator i;
			for(i = childs.begin(); i != childs.end(); i++ ){
				if(orientation == HORIZONTAL){
					if((*i)->getFillParentVertical()){
						(*i)->setHeight(getHeight());
					}
					if((*i)->getFillParentHortizonal()){
						(*i)->setWidth(((getWidth()-totalSpacing)*(*i)->getWeight())/weightSum);
					}
				}else{
					if((*i)->getFillParentHortizonal()){
						(*i)->setWidth(getWidth());
					}
					if((*i)->getFillParentVertical()){
						(*i)->setHeight(((getHeight()-totalSpacing)*(*i)->getWeight())/weightSum);
					}
				}
			}

			int x_start;
			int y_start;

			if((alignment & ALIGN_CENTER_VERTICAL) == ALIGN_TOP){
				y_start = getY();
			}
			else if((alignment & ALIGN_CENTER_VERTICAL) == ALIGN_BOTTOM){
				y_start = getY()+getHeight()-totalSpacing-getChildsHeight();
			}
			else{
				y_start = getY()+(getHeight()-totalSpacing-getChildsHeight())/2;
			}

			if((alignment & ALIGN_CENTER_HORIZONTAL) == ALIGN_LEFT){
				x_start = getX();
			}
			else if((alignment & ALIGN_CENTER_HORIZONTAL) == ALIGN_RIGHT){
				x_start = getX()+getWidth()-totalSpacing-getChildsWidth();
			}
			else{
				x_start = getX()+(getWidth()-totalSpacing-getChildsWidth())/2;
			}

			int xc;
			int yc;
			list<View*>::iterator prev;
			for(i = childs.begin(); i != childs.end(); i++ )
			{
				if(orientation == HORIZONTAL){
					if(i == childs.begin()){
						xc=x_start;
					}else{
						xc=(*prev)->x+(*prev)->getWidth()+spacing;
					}

					if((alignment & ALIGN_CENTER_VERTICAL) == ALIGN_TOP){
						yc=getY();
					}
					else if((alignment & ALIGN_CENTER_VERTICAL) == ALIGN_BOTTOM){
						yc=getY()+getHeight()-(*i)->getHeight();
					}
					else{
						yc=getY()+(getHeight()-(*i)->getHeight())/2;
					}
				}else{
					if(i == childs.begin()){
						yc=y_start;
					}else{
						yc=(*prev)->y+(*prev)->getHeight()+spacing;
					}

					if((alignment & ALIGN_CENTER_HORIZONTAL) == ALIGN_LEFT){
						xc=getX();
					}
					else if((alignment & ALIGN_CENTER_HORIZONTAL) == ALIGN_RIGHT){
						xc=getX()+(getWidth()-(*i)->getWidth())/2;
					}
					else{
						xc=getX()+(getWidth()-(*i)->getWidth())/2;
					}
				}

				(*i)->setPosition(xc,yc);
				prev=i;
			}
			//__super::updateLayout();
#ifndef NDEBUG
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}

int LinearLayout::getChildsWeight(){
	int w=0;
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		w+= (*i)->getWeight();
	}
	return w;
}

int LinearLayout::getChildsHeight(){
	int h=0;
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		h+= (*i)->getHeight();
	}
	return h;
}

int LinearLayout::getChildsWidth(){
	int w=0;
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		w+= (*i)->getWidth();
	}
	return w;
}

void LinearLayout::setOrientation(int o){
	orientation=o;
	need_update_layout = 1;
}

void LinearLayout::setSpacing(unsigned int s){
	spacing = s;
}

void LinearLayout::setAlignment(unsigned int a){
	alignment = a;
	need_update_layout = 1;
}

int LinearLayout::getOrientation(){
	return orientation;
}

void LinearLayout::draw(SDL_Surface * display, int delay){
	if(this->need_update_layout == 1){
		this->updateLayout();
		this->need_update_layout=0;
	}
	ViewGroup::draw(display, delay);
}

LinearLayout::~LinearLayout(void)
{
}
