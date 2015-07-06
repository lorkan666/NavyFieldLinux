/*
 * ConnectionInfoList.cpp
 *
 *  Created on: 27 cze 2015
 *      Author: lorkan
 */

#include "ConnectionInfoList.h"
#include <iostream>

ConnectionInfoList::ConnectionInfoList() {
}

bool ConnectionInfoList::contains(Address adr) {
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr )
			return true;
	return false;
}

ConnectionInfo* ConnectionInfoList::get(Address adr) {
	list<ConnectionInfo>::iterator itor;
	for ( itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr )
			return &(*itor);
	return NULL;
}

ConnectionInfoList::~ConnectionInfoList() {
}

void ConnectionInfoList::remove(Address adr) {
	list<ConnectionInfo>::iterator itor;
	for ( itor = begin(); itor != end(); ++itor )
		if ( itor->address == adr ){
			this->erase(itor);
			break;
		}
}
