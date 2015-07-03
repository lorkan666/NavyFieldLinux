/*
 * ConnectionInfo.cpp
 *
 *  Created on: 27 cze 2015
 *      Author: lorkan
 */

#include "ConnectionInfo.h"
#include <iostream>
#include <map>
#include "AckSystem.h"
using namespace std;


ConnectionInfo::ConnectionInfo() {
	mode = None;
	state = Disconnected;
	timeoutAccumulator = 0.0;
	cl=NULL;
	acks_sys = AckSystem(0xFFFFFFFF);
}

void ConnectionInfo::setState(State s){
	cout<<"Address "<<this->address.GetAddress()<<" z "<<state<< " na "<<s<<endl;
	state = s;
	switch(state){
	case Disconnected:
		onDisconnect(*this);
		break;
	case Connected:
		onConnect(*this);
		break;
	case ConnectFail:
		onConnectFailed(*this);
		break;
	}
}

ConnectionInfo::~ConnectionInfo() {
	if(cl != NULL)
		delete cl;
}


void ConnectionInfo::onConnect(ConnectionInfo &ci) {
	if(cl != NULL)
		cl->onConnect(ci);
}

void ConnectionInfo::onDisconnect(ConnectionInfo &ci) {
	if(cl != NULL)
		cl->onDisconnect(ci);
}

void ConnectionInfo::onConnectFailed(ConnectionInfo &ci) {
	if(cl != NULL)
		cl->onConnectFailed(ci);
}

void ConnectionInfo::setConnectionListener(ConnectionListener * cl) {
	this->cl=cl;
}

