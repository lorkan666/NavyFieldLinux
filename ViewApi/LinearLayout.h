#pragma once
#include "ViewGroup.h"

class LinearLayout :
	public ViewGroup
{
public:
	static const int HORIZONTAL = 1;
	static const int VERTICAL = 0;
	LinearLayout(void);
	virtual void draw(SDL_Surface *, int);
	void setOrientation(int);
	void setAlignment(unsigned int a);
	void setSpacing(unsigned int s);
	int getOrientation(void);
	~LinearLayout(void);
	const static unsigned int ALIGN_TOP = 1;
	const static unsigned int ALIGN_BOTTOM = 2;
	const static unsigned int ALIGN_LEFT = 4;
	const static unsigned int ALIGN_RIGHT = 8;

	const static unsigned int ALIGN_CENTER_VERTICAL = 3;
	const static unsigned int ALIGN_CENTER_HORIZONTAL = 12;
private:

	int getChildsWeight();
	int getChildsWidth();
	int getChildsHeight();

protected:
	int orientation;
	unsigned int spacing;
	unsigned int alignment;
	virtual void updateLayout();

};

