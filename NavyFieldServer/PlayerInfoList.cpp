/*
 * PlayerInfoList.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include "PlayerInfoList.h"

PlayerInfoList::PlayerInfoList() {
}

bool PlayerInfoList::contains(Address adr) {
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr )
			return true;
	return false;
}
