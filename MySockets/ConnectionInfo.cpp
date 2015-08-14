/*
 * ConnectionInfo.cpp
 *
 *  Created on: 27 cze 2015
 *      Author: lorkan
 */

#include "ConnectionInfo.h"
#include "ConnectionListener.h"
#include <iostream>
#include <map>
#include "AckSystem.h"
using namespace std;


ConnectionInfo::ConnectionInfo() {
	mode = None;
	state = Disconnected;
	timeoutAccumulator = 0.0;
	keeping_alive=false;
	cl=NULL;
	acks_sys = AckSystem(0xFFFFFFFF);
}

void ConnectionInfo::setState(State s){
	cout<<"Address "<<this->address.GetAddress()<<" z "<<state<< " na "<<s<<endl;
	state = s;
	switch(state){
	case Disconnected:
		if(cl != NULL)
			cl->onDisconnect(*this);
		break;
	case Connected:
		if(cl != NULL)
			cl->onConnect(*this);
		break;
	case ConnectFail:
		if(cl != NULL)
			cl->onConnectFailed(*this);
		break;
	}
}

ConnectionInfo::~ConnectionInfo() {
}

void ConnectionInfo::setConnectionListener(ConnectionListener * cl) {
	this->cl=cl;
}

