/*
 * PlayerInfo.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include "PlayerInfo.h"
#include <cstring>

PlayerInfo::PlayerInfo(string login, Address adr) {
	this->login = login;
	address = adr;
	ship=0;
}

PlayerInfo::PlayerInfo() {
	login="";
	ship=0;
}

void PlayerInfo::selectShip(int ship) {
	this->ship = ship;
}
