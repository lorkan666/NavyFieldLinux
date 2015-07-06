/*
 * GameProtocol.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef GAMEPROTOCOL_H_
#define GAMEPROTOCOL_H_
#include "MyPacket.h"

class LoginPacket: public MyPacket{
public:
	char* login;
	char* password;
	LoginPacket(MyPacket & p){
		*this = p;
		login = this->getDataPointer();
		password = this->getDataPointer()+20;
	}
};


#endif /* GAMEPROTOCOL_H_ */
