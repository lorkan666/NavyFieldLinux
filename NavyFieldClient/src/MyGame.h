/*
 * MyGame.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef MYGAME_H_
#define MYGAME_H_

#include <Game.h>
#include "MySockets.h"

class MyUDPConnection;
class MyGame: public Game {
public:
	MyUDPConnection *connection;
	Address server_address;
	virtual void start();
	MyGame();
	virtual ~MyGame();
};

#endif /* MYGAME_H_ */
