#ifndef view_h
#define view_h

#include <SDL/SDL.h>
#include <list>
#include <iostream>

#include "OnKeyboardEventListener.h"
#include "OnMouseEventListener.h"
#include <functional>
using namespace std;

class ViewGroup;
class View: public OnMouseEventListener, public OnKeyboardEventListener
{

public:
	//typedef void (*OnMouseClickCallback)(int x, int y, View * v) ;

	typedef std::function<void (int x, int y, View * v)> OnMouseClickCallback;
	typedef void (*OnKeyCallback)(SDL_KeyboardEvent key, View * v) ;

	friend class ViewGroup;

	int x;
	int y;
	
	virtual void draw(SDL_Surface * display, int delay);
	//virtual void draw(SDL_Surface * display,int x,int y, int delay);
	virtual int getHeight();
	virtual int getWidth();
	virtual void onMouseDown(int x, int y, View * v) ;
	virtual void onClick(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onMouseEnter(int x, int y, View * v) ;
	virtual void onMouseLeave(int x, int y, View * v) ;
	virtual void onMouseMove(int x, int y,int xr, int yr, View * v);
	virtual void onMouseWheelDown(int x, int y, View * v);
	virtual void onMouseWheelUp(int x, int y, View * v);

	virtual void onKeyDown(SDL_KeyboardEvent key , View * v);
	virtual void onKeyUp(SDL_KeyboardEvent key, View * v);

	void setOnMouseEventListener(OnMouseEventListener *);
	void setOnKeyboardEventListener(OnKeyboardEventListener *);
	virtual void setPosition(int x, int y);
	bool inView(int x, int y);
	virtual void setHeight(int);
	virtual void setWidth(int);
	void setWeight(int w);
	int getWeight();
	void setName(string name);
	virtual void setOffset(int x, int y);
	int getX();
	int getY();
	virtual void resize(int width,int height);
	void setFillParent(bool hortizonal, bool vertical);
	bool getFillParentHortizonal();
	bool getFillParentVertical();
	void setOnMouseClickCallback(OnMouseClickCallback callback);
	void setOnKeDownCallback(OnKeyCallback callback);
	void setPadding(int padTop, int padLeft, int padBottom, int padRight);
	void setBackgroundColor(int,int,int,int);
	void setBackgroundColor(Uint32 c);
	ViewGroup * getParent();

	static double getAngle(double fromX,double fromY,double toX,double toY);
	static double getDistance(double fromX,double fromY,double toX,double toY);

	View(void);
	View(int, int);
	virtual ~View(void);
	

protected:
	OnMouseClickCallback onMouseClickCallback;
	OnKeyCallback onKeyDownCallback;
	OnKeyCallback onKeyUpCallback;
	string name;
	int height;
	int width;
	int offsetX;
	int offsetY;
	int weight;
	int padTop;int padBottom;int padLeft;int padRight;
	bool fillParentHorizontal;
	bool fillParentVertical;
	ViewGroup * parent;
	Uint32 backgroundColor;
	OnMouseEventListener * onMouseEventListener;
	OnKeyboardEventListener * onKeyboardEventListener;
	bool wasMouseDown;
	bool wasInView;
};



#endif

