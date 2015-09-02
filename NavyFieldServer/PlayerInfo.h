/*
 * PlayerInfo.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_
#include "Address.h"
#include "ConnectionInfo.h"

class PlayerInfo {
public:
	PlayerInfo();
	PlayerInfo(string login, Address adr);
	string login;
	int ship;
	Address address;
	void selectShip(int ship);
};

#endif /* PLAYERINFO_H_ */
