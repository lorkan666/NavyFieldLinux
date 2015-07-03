#include "LoginScreen.h"

#include "EditView.h"
#include "ImageView.h"
#include "LinearLayout.h"
#include "PasswordView.h"
#include "SimpleButton.h"
#include "TextView.h"
#include "MyUDPConnection.h"


TextView * logLabel, * passLabel;
EditView * logEdit,* passEdit;
SimpleButton * loginButton;
ImageView * bgd;
LinearLayout * loginBox;
View * server_status;

LoginScreen::LoginScreen(void)
{
	
}

LoginScreen::LoginScreen(string name):GameScreen(name)
{

}


void LoginScreen::enter(){

	bgd = new ImageView(game->getStringProperty("login_img"));
	bgd->resize(stage->getWidth(),stage->getHeight());
	stage->addView(bgd);

	server_status = new View();
	server_status->resize(stage->getWidth()*0.01,stage->getWidth()*0.01);
	server_status->setBackgroundColor(255,100,100,1);

	loginBox = new LinearLayout();
	loginBox->resize(stage->getWidth()/4,stage->getHeight()/2);
	loginBox->setPosition(50,50);
	loginBox->setSpacing(5);
	loginBox->setOrientation(LinearLayout::VERTICAL);
	loginBox->setAlignment(LinearLayout::ALIGN_LEFT|LinearLayout::ALIGN_TOP);

	logLabel = new TextView("Login:");
	passLabel = new TextView("Password:");
	logEdit = new EditView();
	logEdit->setSize(12);
	logEdit->setFillParent(true,false);
	logEdit->setText(game->getStringProperty("player_name"));
	passEdit = new PasswordView();
	passEdit->setSize(12);
	passEdit->setFillParent(true,false);
	passEdit->setText("polifons");
	loginButton = new SimpleButton("login");

	loginBox->addView(server_status);
	loginBox->addView(logLabel);
	loginBox->addView(logEdit);
	loginBox->addView(passLabel);
	loginBox->addView(passEdit);
	loginBox->addView(loginButton);

	stage->addView(loginBox);
	game->connection->connect(game->server_address);

}

void LoginScreen::render(SDL_Surface * display, int delay)
{
	if(game->connection->isConnected(game->server_address)){
		server_status->setBackgroundColor(100,255,100,1);
	}else{
		server_status->setBackgroundColor(255,100,100,1);
	}
	stage->draw(display,delay);
}


LoginScreen::~LoginScreen(void)
{
}
