#ifndef on_mouse_event_listener_h
#define on_mouse_event_listener_h

class View;
class OnMouseEventListener{
	public:
		virtual void onMouseDown(int x, int y, View * v) = 0;
		virtual void onClick(int x, int y, View * v) = 0;
		virtual void onMouseUp(int x, int y, View * v) = 0;
		virtual void onMouseMove(int x, int y, int xr, int yr, View * v) = 0;
		virtual void onMouseEnter(int x, int y, View * v) =0;
		virtual void onMouseLeave(int x, int y, View * v) =0;
		virtual void onMouseWheelDown(int x, int y, View * v) =0;
		virtual void onMouseWheelUp(int x, int y, View * v) =0;
};


#endif
