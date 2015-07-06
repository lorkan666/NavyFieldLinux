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
	this->keeping_alive = false;
	this->setMode(ConnectionInfo::Server);

	std::stringstream ss;
	ss << std::hex << getStringProperty("protocol_id");
	ss >> this->protocol_id;

	this->start(Address(0,getIntProperty("30000")));
}

void Portier::onPacketSent(MyPacket p, Address address) {
}

void Portier::onPacketNoneSent(MyPacket p, Address address) {
}

void Portier::onPacketReceived(MyPacket p, Address address) {
	if(!loged_players.contains(address)){ // pierwszy pakiet musi być pakietem logowania
		LoginPacket lp = LoginPacket(p);
		if(isPlayerExist(lp)){
			PlayerInfo pi = PlayerInfo(lp.login,address);
			loged_players.push_back(pi);
		}else{
			this->disconnect(address);
		}
		return;
	}
}

void Portier::onPacketDelivered(MyPacket p, Address address) {
}

void Portier::onPacketLost(MyPacket p, Address address) {
}

Portier::~Portier() {
	stop();
}

bool Portier::isPlayerExist(LoginPacket lp) {
	return true;
}
