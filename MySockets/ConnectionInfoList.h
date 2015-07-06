/*
 * ConnectionInfoList.h
 *
 *  Created on: 27 cze 2015
 *      Author: lorkan
 */

#ifndef CONNECTIONINFOLIST_H_
#define CONNECTIONINFOLIST_H_

#include "ConnectionInfo.h"
#include <list>
using namespace std;

class ConnectionInfoList: public list<ConnectionInfo> {
public:
	ConnectionInfoList();
	bool contains(Address adr);
	ConnectionInfo * get(Address adr);
	void remove(Address adr);
	virtual ~ConnectionInfoList();
};

#endif /* CONNECTIONINFOLIST_H_ */
