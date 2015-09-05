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
		this->writeInt((unsigned char *)getDataPointer()+pos, value);
		pos+=4;
		size+=4;
		return *this;
	}

	GamePacket & addChar(char value){
		if(getHeaderSize()+pos+1 >= getMaxSize())
			return *this;
		getDataPointer()[pos]=value;
		pos+=1;
		size+=1;
		return *this;
	}
	GamePacket & addUChar(unsigned char value){
		if(getHeaderSize()+pos+1 >= getMaxSize())
			return *this;
		((unsigned char*)getDataPointer())[pos]=value;
		pos+=1;
		size+=1;
		return *this;
	}
	GamePacket & addString(string value){
		if(getHeaderSize()+pos+((int)value.length()) >= getMaxSize())
			return *this;
		memcpy(getDataPointer()+pos,value.c_str(),value.size());
		size+=value.size();
		return *this;
	}
private:
	int pos;
};

// C1 | login | password
// 1  |   20  |  20

class LoginPacket: public MyPacket{
public:

	LoginPacket(string login, string password):MyPacket(){
		unsigned char * data_pointer = (unsigned char * )getDataPointer();
		*data_pointer = 0xC1;
		memcpy(data_pointer+1,login.c_str(),login.size());
		/*
		 * szyfrowanie hasla
		 */
		memcpy(data_pointer+21,password.c_str(),password.size());
		size+=41;
	}
};


#endif /* GAMEPROTOCOL_H_ */
