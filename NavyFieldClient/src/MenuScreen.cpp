#include "MenuScreen.h"
#include <SDL/SDL_image.h>
#include <iostream>

#include "EditView.h"
#include "ImageView.h"
#include "LinearLayout.h"
#include "PlayerControler.h"
#include "SimpleButton.h"
#include "TextView.h"

LinearLayout * hbox;

MenuScreen::MenuScreen(string name):GameScreen(name)
{
}

void MenuScreen::enter(){
	hbox = new LinearLayout();
	hbox->resize(stage->getWidth(),stage->getHeight());
	hbox->setSpacing(20);
	hbox->setOrientation(LinearLayout::VERTICAL);
	hbox->setBackgroundColor(0,200,0,1);
	stage->addView(hbox);

	SimpleButton * connectButton = new SimpleButton("Connect");
	//connectButton->setOnMouseClickCallback([](int x, int y, View * v){});
	SimpleButton * settingsButton = new SimpleButton("Settings");
	SimpleButton * quitButton = new SimpleButton("Quit");
	//quitButton->setOnMouseClickCallback([](int x, int y, View * v){});
		
	hbox->addView(connectButton);
	hbox->addView(settingsButton);
	hbox->addView(quitButton);
}

void MenuScreen::render(SDL_Surface * display, int delay){
	GameScreen::render(display,delay);
}

MenuScreen::~MenuScreen(void)
{
}
