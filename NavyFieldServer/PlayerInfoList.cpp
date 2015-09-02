/*
 * PlayerInfoList.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include "PlayerInfoList.h"
#include <iostream>

PlayerInfoList::PlayerInfoList() {
}

bool PlayerInfoList::contains(Address adr) {
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr)
			return true;
	return false;
}

PlayerInfo* PlayerInfoList::get(Address adr) {
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr)
			return &(*itor);
	return NULL;
}

void PlayerInfoList::removeByAddress(Address adr) {
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr){
			this->erase(itor);
			break;
		}
}
