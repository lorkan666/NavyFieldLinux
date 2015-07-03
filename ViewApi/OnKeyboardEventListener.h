#ifndef on_keyboard_event_listener_h
#define on_keyboard_event_listener_h
#include <SDL/SDL.h>

class View;
class OnKeyboardEventListener{
	public:
		virtual void onKeyDown(SDL_KeyboardEvent key , View * v) = 0;
		virtual void onKeyUp(SDL_KeyboardEvent key, View * v) = 0;
};


#endif
