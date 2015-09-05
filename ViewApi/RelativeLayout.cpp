#include "RelativeLayout.h"


RelativeLayout::RelativeLayout(void)
{
}

void RelativeLayout::updateLayout(){
#ifdef _UPDATE
	        Uint32 startTicks = SDL_GetTicks();
#endif
			for( std::list<View *>::iterator iter = childs.begin(); iter != childs.end(); ++iter )
			{
				(*iter)->setOffset(getX(),getY());
			}
#ifdef _UPDATE
			cout<<__FUNCTION__<<"-"<<SDL_GetTicks() - startTicks<<endl;
#endif
}

void RelativeLayout::draw(SDL_Surface * display, int delay){
	#ifdef _DRAW
		Uint32 startTicks = SDL_GetTicks();
	#endif
	if(this->need_update_layout == 1){
		this->updateLayout();
		this->need_update_layout=0;
	}
	if(display ==NULL)
		return;
	View::draw(display, delay);
	list<View*>::iterator i;
	for(i = childs.begin(); i != childs.end(); i++ )
	{
		(*i)->draw(display, delay);
	}
	#ifdef _DRAW
		cout<<__FUNCTION__<<" - " <<SDL_GetTicks() - startTicks<<endl;
	#endif
}

RelativeLayout::~RelativeLayout(void)
{
}
