#include "ViewGroup.h"

#include <iostream>

using namespace std;

ViewGroup::ViewGroup()
{
	this->need_update_layout=1;
}

void ViewGroup::addView(View * v){
	if(v == NULL)
		throw string("Error in: "+string(__PRETTY_FUNCTION__)+" details: View is NULL");
	childs.push_back(v);
	v->parent = this;
	this->need_update_layout=1;
}

void ViewGroup::addView(View* v, int pos) {
	if(v == NULL)
		throw string("Error in: "+string(__PRETTY_FUNCTION__)+" details: View is NULL");
	std::list<View *>::iterator iter;
	iter = childs.begin();
	while(pos>0 && iter != childs.end()){
		iter++;
		pos--;
	}
	childs.insert(iter,v);
	v->parent = this;
	this->need_update_layout=1;
}

void ViewGroup::removeView(View * v){
	childs.remove(v);
	this->need_update_layout=1;
}

void ViewGroup::removeViewAndDelete(View * v){
	childs.remove(v);
	delete v;
	this->need_update_layout=1;
}

void ViewGroup::setOffset(int offsetX, int offsetY){
	ViewParent::setOffset(offsetX, offsetY);

	this->need_update_layout=1;
}

View * ViewGroup::findViewByName(string name){
	ViewGroup *vg;
	View * dvg;
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{
		if( (*iter)->name.compare(name) == 0)
		{
			return *iter;
		}
		vg = dynamic_cast<ViewGroup*>(*iter);
		if(vg == NULL){
			return NULL;
		}
		dvg = vg->findViewByName(name);
		if(dvg != NULL){
			return dvg;
		}
	}
	return NULL;
}

void ViewGroup::updateLayout(){
#ifndef NDEBUG
	        Uint32 startTicks = SDL_GetTicks();
#endif

#ifndef NDEBUG
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}

void ViewGroup::resize(int width,int height){
	View::resize(width,height);
	need_update_layout = 1;
}

void ViewGroup::setPosition(int x, int y){
	View::setPosition(x,y);
	need_update_layout = 1;
}

void ViewGroup::onMouseDown(int x, int y, View* p){
	View::onMouseDown(x,y,this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onMouseDown( x, y, this);
	}	
}

void ViewGroup::onMouseUp(int x, int y, View* p){
	View::onMouseUp(x,y,this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{	
		(*iter)->onMouseUp( x, y, this);
	}	
}

void ViewGroup::onMouseMove(int x, int y, int xr, int yr, View * p){
	View::onMouseMove(x, y, xr, yr, this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onMouseMove(x, y, xr, yr, this);
	}
}

void ViewGroup::onMouseWheelDown(int x, int y, View * v){
	View::onMouseWheelDown(x, y, this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onMouseWheelDown(x, y,this);
	}
}
void ViewGroup::onMouseWheelUp(int x, int y, View * v){
	View::onMouseWheelUp(x, y, this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onMouseWheelUp(x, y,this);
	}
}

void ViewGroup::onKeyDown(SDL_KeyboardEvent key , View * v) {
	View::onKeyDown(key, this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onKeyDown(key, this);
	}
}

void ViewGroup::onKeyUp(SDL_KeyboardEvent key, View * v) {
	View::onKeyUp(key, this);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{		
		(*iter)->onKeyUp(key, this);
	}
}

void ViewGroup::draw(SDL_Surface * display, int delay){

	if(this->need_update_layout == 1){
		this->updateLayout();
		this->need_update_layout=0;
	}
	View::draw(display, delay);
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{
		(*iter)->draw(display, delay);
	}
}


ViewGroup::~ViewGroup(void)
{
	for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
	{
		if(*iter != NULL)
			delete *iter;
	}
}

