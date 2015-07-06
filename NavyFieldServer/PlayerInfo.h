/*
 * PlayerInfo.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef PLAYERINFO_H_
#define PLAYERINFO_H_
#include "Address.h"

class PlayerInfo {
public:
	PlayerInfo(const char* login, Address address);
	char login[20];
	char ship[20];
	Address address;
};

#endif /* PLAYERINFO_H_ */
