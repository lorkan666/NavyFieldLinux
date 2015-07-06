/*
 * MyGame.cpp
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#include <MyGame.h>
#include <sstream>

MyGame::MyGame():Game(){
	unsigned int protocol_id;
	std::stringstream ss;
	ss << std::hex << getStringProperty("protocol_id");
	ss >> protocol_id;
	cout<<getStringProperty("protocol_id")<<endl;

	connection = new MyUDPConnection(protocol_id);
	connection->setTimeout(getDoubleProperty("timeout"));
	connection->keepAlive(getBoolProperty("keep_alive"));
	connection->setMode(ConnectionInfo::Client);
	server_address = Address(getStringProperty("server_ip"),(unsigned int)getIntProperty("server_port"));
}

void MyGame::start() {
	connection->start(Address(0,getIntProperty("client_port")));
	Game::start();
}

MyGame::~MyGame() {
	if(connection != NULL)
		delete connection;
}

