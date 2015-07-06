/*
 * PlayerInfo.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include "PlayerInfo.h"
#include <cstring>

PlayerInfo::PlayerInfo(const char* login, Address address) {
	memcpy(this->login, login, 20);
	this->address=address;
}
