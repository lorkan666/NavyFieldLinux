/*
 * PortScreen.cpp
 *
 *  Created on: 2 wrz 2015
 *      Author: lorkan
 */

#include <PortScreen.h>
#include "LinearLayout.h"
#include "SimpleButton.h"

LinearLayout * vbox, * mainbox;
LinearLayout * top_menu, * right_menu, * left_menu;

PortScreen::PortScreen(void) {
}

PortScreen::PortScreen(string name):GameScreen(name) {
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
	//connectButton->setOnMouseClickCallback([](int x, int y, View * v){});
	SimpleButton * mushButton = new SimpleButton("Musashi");
	mainbox->addView(iowaButton);
	mainbox->addView(mushButton);

	vbox->addView(top_menu);
	vbox->addView(mainbox);

	stage->addView(vbox);
}

void PortScreen::leave() {
}
