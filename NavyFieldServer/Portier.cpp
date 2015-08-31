/*
 * Portier.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include "Portier.h"
#include <sstream>
using namespace std;

Portier::Portier():MyUDPConnection() {
	readProperties("server.xml");
	this->setTimeout(getDoubleProperty("timeout"));
	this->keeping_alive = getBoolProperty("keep_alive");
	this->setMode(ConnectionInfo::Server);

	std::stringstream ss;
	ss << std::hex << getStringProperty("protocol_id");
	ss >> this->protocol_id;
	cout<<getStringProperty("protocol_id")<<endl;

	this->start(Address(0,getIntProperty("server_port")));
}

void Portier::onPacketSent(MyPacket p, Address address) {
	MyUDPConnection::onPacketSent( p, address);
}

void Portier::onPacketNoneSent(MyPacket p, Address address) {
	MyUDPConnection::onPacketNoneSent( p, address);
}

void Portier::onPacketReceived(MyPacket p, Address address) {
	unsigned char code = *p.getDataPointer();
	switch(code){
	case 0xC1:
		LoginPacket lp = LoginPacket(p);
		if(isPlayerExist(lp.login, lp.password)){
			cout<<"login:"<<lp.login<<" ("<<lp.login.length()<<")"<<endl;
			virtual_connections.get(address)->keeping_alive = true;
		}
		break;
	}
	MyUDPConnection::onPacketReceived( p, address);
}

void Portier::onPacketDelivered(MyPacket p, Address address) {
	MyUDPConnection::onPacketDelivered( p, address);
}

void Portier::onPacketLost(MyPacket p, Address address) {
	MyUDPConnection::onPacketLost( p, address);
}

Portier::~Portier() {
	stop();
}

void Portier::onConnect(ConnectionInfo& ci) {
}

void Portier::onDisconnect(ConnectionInfo& ci) {
}

void Portier::onConnectFailed(ConnectionInfo& ci) {
}

bool Portier::isPlayerExist(string name, string hashed_password) {
	return name.length() == 6;
}
