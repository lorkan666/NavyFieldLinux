/*
 * PortScreen.cpp
 *
 *  Created on: 2 wrz 2015
 *      Author: lorkan
 */

#include <PortScreen.h>
#include "LinearLayout.h"
#include "SimpleButton.h"
#include "MyGame.h"
#include "GameProtocol.h"

LinearLayout * vbox, * mainbox;
LinearLayout * top_menu, * right_menu, * left_menu;

PortScreen::PortScreen(void) {
	selected_ship=0;
}

PortScreen::PortScreen(string name):GameScreen(name) {
	selected_ship=0;
}

void PortScreen::render(SDL_Surface* display, int delay) {
	stage->draw(display,delay);
}

void PortScreen::enter() {
	vbox = new LinearLayout();
	vbox->resize(stage->getWidth(),stage->getHeight());

	vbox->setOrientation(LinearLayout::VERTICAL);
	vbox->setBackgroundColor(100,200,0,1);

	top_menu = new LinearLayout();
	top_menu->setOrientation(LinearLayout::HORIZONTAL);
	top_menu->setBackgroundColor(10,10,10,1);
	top_menu->setFillParent(true,true);
	top_menu->setWeight(1);

	left_menu = new LinearLayout();
	left_menu->setFillParent(true,true);
	left_menu->setWeight(5);
	//left_menu->setBackgroundColor(220,10,10,1);
	left_menu->setAlignment(LinearLayout::ALIGN_CENTER_HORIZONTAL|LinearLayout::ALIGN_CENTER_VERTICAL);
	SimpleButton * battleButton = new SimpleButton("Battle");
	left_menu->addView(battleButton);

	right_menu = new LinearLayout();
	//right_menu->setBackgroundColor(10,220,10,1);
	right_menu->setAlignment(LinearLayout::ALIGN_CENTER_HORIZONTAL|LinearLayout::ALIGN_CENTER_VERTICAL);
	right_menu->setFillParent(true,true);
	right_menu->setWeight(1);
	SimpleButton * logoutButton = new SimpleButton("LogOut");
	right_menu->addView(logoutButton);


	top_menu->addView(left_menu);
	top_menu->addView(right_menu);

	mainbox = new LinearLayout();
	mainbox->setOrientation(LinearLayout::VERTICAL);
	mainbox->setAlignment(LinearLayout::ALIGN_CENTER_HORIZONTAL|LinearLayout::ALIGN_CENTER_VERTICAL);
	mainbox->setBackgroundColor(110,100,110,1);
	mainbox->setFillParent(true,true);
	mainbox->setSpacing(20);
	mainbox->setWeight(8);

	SimpleButton * iowaButton = new SimpleButton("Iowa");
	SimpleButton * mushButton = new SimpleButton("Musashi");
	mainbox->addView(iowaButton);
	mainbox->addView(mushButton);

	vbox->addView(top_menu);
	vbox->addView(mainbox);

	stage->addView(vbox);

	MyGame *mg = (MyGame*)game;
	mg->connection->setConnectionListener(this);
	mg->connection->setPacketListener(this);
	iowaButton->setOnMouseClickCallback(
			[mg](int x, int y, View * v){
				MyPacket p = GamePacket().addUChar(0xC2).addUInt(1);
				p.setRetransmiting(3);
				mg->connection->connect(mg->server_address);
				mg->connection->sendTask(p,mg->server_address);
			});
	mushButton->setOnMouseClickCallback(
				[mg](int x, int y, View * v){
					MyPacket p = GamePacket().addUChar(0xC2).addUInt(2);
					p.setRetransmiting(3);
					mg->connection->connect(mg->server_address);
					mg->connection->sendTask(p,mg->server_address);
				});
	battleButton->setOnMouseClickCallback(
				[mg](int x, int y, View * v){
					MyPacket p = GamePacket().addUChar(0xC3);
					p.setRetransmiting(3);
					mg->connection->connect(mg->server_address);
					mg->connection->sendTask(p,mg->server_address);
				});
	logoutButton->setOnMouseClickCallback(
				[mg](int x, int y, View * v){
					MyPacket p = GamePacket().addUChar(0xC4);
					p.setRetransmiting(3);
					mg->connection->connect(mg->server_address);
					mg->connection->sendTask(p,mg->server_address);
				});
}

void PortScreen::leave() {
}

void PortScreen::onConnect(ConnectionInfo& ci) {
}

void PortScreen::onConnecting(ConnectionInfo& ci) {
}

void PortScreen::onDisconnect(ConnectionInfo& ci) {
	MyGame *mg = (MyGame*)game;
	mg->changeScreen("login");
}

void PortScreen::onConnectFailed(ConnectionInfo& ci) {
}

void PortScreen::onPacketSent(MyPacket p, Address address) {
}

void PortScreen::onPacketNoneSent(MyPacket p, Address address) {
}

void PortScreen::onPacketReceived(MyPacket p, Address address) {
	unsigned char * code = (unsigned char * )p.getDataPointer();
	switch(*code){
	case 0xC3:{
			if(*(code+1) == 1)
				this->game->changeScreen("battle");
			else{
				//nie wybrano okretu
			}
			break;
		}
	}
}

void PortScreen::onPacketDelivered(MyPacket p, Address address) {
}

void PortScreen::onPacketLost(MyPacket p, Address address) {
}
