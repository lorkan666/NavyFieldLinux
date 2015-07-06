/*
 * GameProtocol.h
 *
 *  Created on: 6 lip 2015
 *      Author: lorkan
 */

#ifndef GAMEPROTOCOL_H_
#define GAMEPROTOCOL_H_
#include "MyPacket.h"

class GamePacket: public MyPacket{
public:
	GamePacket():MyPacket(){
		pos=0;
	}

	GamePacket & addUInt(unsigned int value){
		if(getHeaderSize()+pos+4 >= getMaxSize())
			return *this;
		this->writeInt((unsigned char *)data+pos, value);
		pos+=4;
		size+=4;
		return *this;
	}

	GamePacket & addChar(unsigned char value){
		if(getHeaderSize()+pos+1 >= getMaxSize())
			return *this;
		data[pos]=value;
		pos+=1;
		size+=1;
		return *this;
	}
	GamePacket & addString(string value){
		if(getHeaderSize()+pos+value.size() >= getMaxSize())
			return *this;
		memcpy(data+pos,value.c_str(),value.size());
		size+=value.size();
		return *this;
	}
private:
	int pos;
};

// C1 | len | login | password
// 1  |  4  |   20  |  20

class LoginPacket: public MyPacket{
public:
	string login;
	string password;
	unsigned int length;
	LoginPacket(MyPacket & p){
		length = MyPacket::readInt((unsigned char*)p.getDataPointer()+1);
		login = string(p.getDataPointer()+5,20);
		password = string(p.getDataPointer()+25,20);
	}

	LoginPacket(string login, string password):MyPacket(){
		*data = 0xC1;
		this->writeInt((unsigned char *)(data+1),40);
		memcpy(data+5,login.c_str(),login.size());
		/*
		 * szyfrowanie hasla
		 */
		memcpy(data+5,password.c_str(),password.size());
		size+=45;
	}
};


#endif /* GAMEPROTOCOL_H_ */
