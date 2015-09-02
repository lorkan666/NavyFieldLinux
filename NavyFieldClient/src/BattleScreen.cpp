#include "BattleScreen.h"

#include "AimingCursor.h"
#include "GameMap.h"
#include "LinearLayout.h"
#include "PlayerControler.h"
#include "RelativeLayout.h"
#include "Ship.h"

BattleScreen::BattleScreen(string name):GameScreen(name)
{
}

void BattleScreen::enter(){
	LinearLayout * vbox = new LinearLayout();
	vbox->resize(stage->getWidth(),stage->getHeight());
	vbox->setOrientation(LinearLayout::VERTICAL);
	stage->addView(vbox);

	GameMap * sea = new GameMap(game->getStringProperty("maps_dir")+"co/");
	sea->setAimBarPath(game->getStringProperty("aim_bar_img"));
	//RelativeLayout * sea = new RelativeLayout();
	sea->setName("sea");
	sea->setWeight(12);
	//sea->setBackgroundColor(110,110,200,1);
	sea->setFillParent(true,true);	

	Ship * iowa = new Ship(game->getStringProperty("ships_directory")+"1/");
	iowa->setName("iowa");
	//iowa->setBackgroundColor(110,110,100,1);
	sea->addView(iowa);
	sea->setPlayerShip(iowa);
	//sea->addView(iowa2);
	vbox->addView(sea);
	//cout<<sea->getWidth()<<","<<sea->getHeight()<<endl;
	iowa->setPosition(300,200);
	//iowa2->setPosition(300,400);
	sea->setOnMouseEventListener(new PlayerControler(sea));
	sea->setOnKeyboardEventListener(new PlayerControler(sea));	
}


BattleScreen::~BattleScreen(void)
{
}
