#ifndef view_parent_h
#define view_parent_h
#include <list>

#include "View.h"

using namespace std;

class ViewParent : public View
{
public:
	virtual void addView(View *) = 0;
	virtual void removeView(View *) = 0;
	void invalidateLayout();
	virtual void invalidableByChilds(bool inv);

protected:
	list<View*> childs;
	bool invalidable;
	int need_update_layout;
	virtual void updateLayout() = 0;
};

#endif

