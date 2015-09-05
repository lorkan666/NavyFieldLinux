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
	this->setConnectionListener(this);

	std::stringstream ss;
	ss << std::hex << getStringProperty("protocol_id");
	ss >> this->protocol_id;
	cout<<getStringProperty("protocol_id")<<endl;
	this->setRttMax(1.0f);
	this->start(Address(0,getIntProperty("server_port")));
}

void Portier::onPacketSent(MyPacket p, Address address) {
	MyUDPConnection::onPacketSent( p, address);
}

void Portier::onPacketNoneSent(MyPacket p, Address address) {
	MyUDPConnection::onPacketNoneSent( p, address);
}

void Portier::onPacketReceived(MyPacket p, Address address) {
	MyUDPConnection::onPacketReceived( p, address);
	unsigned char * code = (unsigned char *)p.getDataPointer();
	switch(*code){
	case 0xC1:{
		if(logged_players.contains(address))
			return;
		string login = string(p.getDataPointer()+1);
		string password = string(p.getDataPointer()+21);
		if(validatePlayer(login, password)){
			MyPacket pp(GamePacket().addUChar(0xC1).addUChar(0x01));
			pp.setRetransmiting(3);
			sendTask(pp,address);
			ConnectionInfo * ci = virtual_connections.get(address);
			ci->keeping_alive = true;
			PlayerInfo pi = PlayerInfo(login, ci->address);
			logged_players.push_back(pi);
			cout<<"Player "<<login<<" logged in"<<endl;
		}else{
			MyPacket pp(GamePacket().addUChar(0xC1).addUChar(0x00));
			pp.setRetransmiting(3);
			sendPacket(pp,address);
		}
		break;
	}
	case 0xC2:{
		if(!logged_players.contains(address))
			return;
		PlayerInfo * pi = logged_players.get(address);
		pi->ship = p.readInt(p.getDataPointer()+1);
		MyPacket pp(GamePacket().addUChar(0xC2).addUChar(0x01));
		pp.setRetransmiting(3);
		sendTask(pp,address);
		break;
	}
	case 0xC3:{
		if(!logged_players.contains(address))
			return;
		PlayerInfo * pi = logged_players.get(address);
		if(pi->ship != 0){
			MyPacket pp(GamePacket().addUChar(0xC3).addUChar(0x01));
			pp.setRetransmiting(3);
			sendTask(pp,address);
		}else{
			MyPacket pp(GamePacket().addUChar(0xC3).addUChar(0x00));
			pp.setRetransmiting(3);
			sendTask(pp,address);
		}
		break;
	}
	case 0xC4:{
		this->disconnect(address);
	}
	}
}

void Portier::onPacketDelivered(MyPacket p, Address address) {
	MyUDPConnection::onPacketDelivered( p, address);
}

void Portier::onPacketLost(MyPacket p, Address address) {
	MyUDPConnection::onPacketLost( p, address);
}

Portier::~Portier() {
	stop();
	logged_players.clear();
}

void Portier::onConnect(ConnectionInfo& ci) {
	cout<<"Connect: "<<ci.address<<endl;
}

void Portier::onDisconnect(ConnectionInfo& ci) {
	logged_players.removeByAddress(ci.address);
	cout<<"Disconnect: "<<ci.address<<endl;
}

void Portier::onConnectFailed(ConnectionInfo& ci) {
}

void Portier::onConnecting(ConnectionInfo& ci) {
}

bool Portier::validatePlayer(string name, string hashed_password) {
	//teorytycznie sprawdzanie w bazie danych
	return name.length() == 6;
}
