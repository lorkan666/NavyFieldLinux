#pragma once
#include "PropertiesConfigReader.h"
#include "RelativeLayout.h"
#include "Ship.h"

class AimingCursor;
class Ship;
class GameMap :
	public RelativeLayout,
	public PropertiesConfigReader
{
public:
	GameMap(string path);
	~GameMap(void);
	virtual void draw(SDL_Surface *, int);
	void setPlayerShip(Ship * s);
	int getMapWidth();
	int getMapHeight();
	void setToInMapMouseXY(int *x, int *y);
	virtual void resize(int width,int height);
	virtual void onMouseDown(int x, int y, View * v) ;
	virtual void onMouseUp(int x, int y, View * v) ;
	virtual void onMouseMove(int x, int y, int xr, int yr, View * v);
	virtual void onMouseEnter(int x, int y, View * v) ;
	virtual void onMouseLeave(int x, int y, View * v) ;
	virtual void onMouseWheelDown(int x, int y, View * v);
	virtual void onMouseWheelUp(int x, int y, View * v);
	void switchFramePos();
	void updateLayout();
	void updateFrameSurface();
	void recreateMapSurface();
	void recreateFrameSurface();
	void zoomTo(int x, int y);
	
	bool isMouseCloseBorder();
	void fixFrame();
	void setAimBarPath(string path);
	void setAimingCursor(AimingCursor * a);
	void fixAimingCursor();
	void fixAimingCursorRotations();
	void fixAimingCursorPosition();
	bool getIsMouseNearBorder() const;
	bool isMoveFrameToTarget() const;

	int lastMouseX,lastMouseY;
	volatile bool isMouseNearBorder;
	Ship * playerShip;

private:
	bool frameToShip;
	volatile bool moveFrameToTarget;
	string aim_bar_img_path;
	static int moveFrame(void* data);
	static const int mouseBorderOffset = 20;
	AimingCursor * aimingCursor;
	SDL_Thread * movementThread;
	SDL_Rect frame_move;
	SDL_Rect frame;
	SDL_Rect frame_original;
	
	double zoom;
	bool zoom_changed;
	SDL_Surface * surface;
	SDL_Surface * frame_surface;
	SDL_Surface * map_surface;
	SDL_Surface * img;
	void repeatImgOnMapSurface();
	Uint32 rmask, gmask, bmask, amask;
	
};

