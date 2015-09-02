/*
 * PortScreen.h
 *
 *  Created on: 2 wrz 2015
 *      Author: lorkan
 */

#ifndef PORTSCREEN_H_
#define PORTSCREEN_H_

#include <GameScreen.h>

class PortScreen: public GameScreen {
public:
	PortScreen(void);
	PortScreen(string name);
	virtual void render(SDL_Surface *, int );
	virtual void enter();
	virtual void leave();
};

#endif /* PORTSCREEN_H_ */
