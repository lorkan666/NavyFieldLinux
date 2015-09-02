#include "LoginScreen.h"

#include "EditView.h"
#include "ImageView.h"
#include "LinearLayout.h"
#include "PasswordView.h"
#include "SimpleButton.h"
#include "TextView.h"
#include "MyUDPConnection.h"
#include "MyGame.h"
#include "MyPacket.h"
#include "GameProtocol.h"

TextView * logLabel, * passLabel;
EditView * logEdit,* passEdit;
SimpleButton * loginButton;
ImageView * bgd;
LinearLayout * loginBox;
int reconnects;
MyGame *mg;

LoginScreen::LoginScreen(void)
{
	
}

LoginScreen::LoginScreen(string name):GameScreen(name)
{
	bgd = NULL;
	loginBox = new LinearLayout();
	logLabel = new TextView("Login:");
	passLabel = new TextView("Password:");
	logEdit = new EditView();
	passEdit = new PasswordView();
	loginButton = new SimpleButton("login");

	loginBox->addView(logLabel);
	loginBox->addView(logEdit);
	loginBox->addView(passLabel);
	loginBox->addView(passEdit);
	loginBox->addView(loginButton);

	stage->addView(loginBox);
}


void LoginScreen::enter(){
	if(bgd == NULL){
		bgd = new ImageView(game->getStringProperty("login_img"));
		stage->addView(bgd,0);
	}

	bgd->resize(stage->getWidth(),stage->getHeight());

	loginBox->resize(stage->getWidth()/4,stage->getHeight()/2);
	loginBox->setPosition(50,50);
	loginBox->setSpacing(5);
	loginBox->setOrientation(LinearLayout::VERTICAL);
	loginBox->setAlignment(LinearLayout::ALIGN_LEFT|LinearLayout::ALIGN_TOP);

	logEdit->setSize(12);
	logEdit->setFillParent(true,false);
	logEdit->setText(game->getStringProperty("player_name"));

	passEdit->setSize(12);
	passEdit->setFillParent(true,false);
	passEdit->setText("polifons");

	mg = (MyGame*)game;
	mg->connection->setConnectionListener(this);
	mg->connection->setPacketListener(this);
	loginButton->setOnMouseClickCallback(
			[mg,&logEdit,&passEdit](int x, int y, View * v){
				MyPacket p = LoginPacket(logEdit->getText(),passEdit->getText());
				mg->connection->connect(mg->server_address);
				mg->connection->sendPacket(p,mg->server_address);
			});
}

void LoginScreen::render(SDL_Surface * display, int delay)
{
	stage->draw(display,delay);
}

void LoginScreen::onConnect(ConnectionInfo& ci) {
	//this->game->changeScreen("port");
}

void LoginScreen::onDisconnect(ConnectionInfo& ci) {
}

void LoginScreen::onConnectFailed(ConnectionInfo& ci) {
}

void LoginScreen::leave() {
}

void LoginScreen::onConnecting(ConnectionInfo& ci) {
}

void LoginScreen::onPacketSent(MyPacket p, Address address) {
}

void LoginScreen::onPacketNoneSent(MyPacket p, Address address) {
}

void LoginScreen::onPacketReceived(MyPacket p, Address address) {
	unsigned char * code = (unsigned char * )p.getDataPointer();
	 int * test = ( int * )p.getDataPointer();
	cout<<*test<<endl;
	switch(*code){
	case 0xC1:{
			this->game->changeScreen("port");
		}
		break;
	}
}

void LoginScreen::onPacketDelivered(MyPacket p, Address address) {
}

void LoginScreen::onPacketLost(MyPacket p, Address address) {
}

LoginScreen::~LoginScreen(void)
{
}
