/*
 * PlayerInfoList.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef PLAYERINFOLIST_H_
#define PLAYERINFOLIST_H_

#include "PlayerInfo.h"
#include <list>
#include <Address.h>

class PlayerInfoList: public std::list<PlayerInfo> {
public:
	PlayerInfoList();
	bool contains(Address adr);
	PlayerInfo * get(Address adr);
	void removeByAddress(Address adr);
};

#endif /* PLAYERINFOLIST_H_ */
