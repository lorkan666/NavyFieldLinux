/*
 * MyPacket.cpp
 *
 *  Created on: 25 cze 2015
 *      Author: lorkan
 */

#include "MyPacket.h"
#include <bitset>

MyPacket::MyPacket() {
	data = new char[MTU];
	delivered = false;
	time=0;
	size = getHeaderSize();
	header = (PacketHeader*)data;
}

MyPacket::MyPacket(const char* msg, unsigned int size):MyPacket() {
	if(size >= (MTU - getHeaderSize()))
		throw string("Packet data to large for MTU.");
	if(msg != NULL)
		memcpy(getDataPointer(), msg, size);
	this->size = size + getHeaderSize();
}

MyPacket::MyPacket(const MyPacket& coped):MyPacket() {
	this->delivered = coped.delivered;
	this->size = coped.size;
	memcpy(this->data, coped.data, coped.size);
}


MyPacket::~MyPacket() {
	delete [] data;
}


bool MyPacket::isDelivered() {
	return delivered;
}

void MyPacket::writeInt( unsigned char * data_char, unsigned int value )
{
	data_char[0] = (unsigned char) ( value >> 24 );
	data_char[1] = (unsigned char) ( (value >> 16) & 0xFF );
	data_char[2] = (unsigned char) ( (value >> 8) & 0xFF );
	data_char[3] = (unsigned char) ( value & 0xFF );
}

void MyPacket::writeInt( unsigned int * data_int, unsigned int value )
{
	unsigned char * data_char = (unsigned char *)data_int;
	data_char[0] = (unsigned char) ( value >> 24 );
	data_char[1] = (unsigned char) ( (value >> 16) & 0xFF );
	data_char[2] = (unsigned char) ( (value >> 8) & 0xFF );
	data_char[3] = (unsigned char) ( value & 0xFF );
}

unsigned int MyPacket::readInt( unsigned char * data_char)
{
	return ( ( (unsigned int)data_char[0] << 24 ) | ( (unsigned int)data_char[1] << 16 ) |
			  ( (unsigned int)data_char[2] << 8 )  | ( (unsigned int)data_char[3] ) );
}

unsigned int MyPacket::readInt( unsigned int * data_int)
{
	unsigned char * data_char = (unsigned char *)data_int;
	return ( ( (unsigned int)data_char[0] << 24 ) | ( (unsigned int)data_char[1] << 16 ) |
			  ( (unsigned int)data_char[2] << 8 )  | ( (unsigned int)data_char[3] ) );
}

const char* MyPacket::getData(){
	return data;
}

unsigned int MyPacket::getAck() {
	return readInt(&(header->ack));
}

void MyPacket::setAck(unsigned int ack) {
	writeInt(&(header->ack),ack);
}

unsigned int MyPacket::getAcksRecent() {
	return readInt(&(header->acks_recent));
}

void MyPacket::setAcksRecent(unsigned int acksRecent) {
	writeInt(&(header->acks_recent),acksRecent);
}

unsigned int MyPacket::getPid(){
	return readInt(&(header->pid));
}

void MyPacket::setPid(unsigned int pid) {
	writeInt(&(header->pid),pid);
}

unsigned int MyPacket::getSequence() {
	return readInt(&(header->sequence));
}

void MyPacket::setSequence(unsigned int sequence) {
	writeInt(&(header->sequence),sequence);
}

int MyPacket::getSize() {
	return size;
}

int MyPacket::getMaxSize() {
	return MTU;
}

int MyPacket::getHeaderSize() {
	return sizeof(PacketHeader);
}

void MyPacket::setSize(int size) {
	this->size = size;
	if(size>MTU)
		this->size = MTU;
}

char* MyPacket::getDataPointer() {
	return data + sizeof(PacketHeader);
}


bool MyPacket::operator ==( const MyPacket & p )
{
	if(this->size != ((MyPacket)p).getSize())
		return false;

	for(int i=0;i<this->size;i++){
		if(this->data[i] != p.data[i])
			return false;
	}
    return true;
}

extern std::ostream & operator <<( std::ostream & s, MyPacket & p )
{
    return s <<"|"<<p.getPid()<<"|"
    		 <<"|"<<p.getSequence()<<"|"
			 <<"|"<<p.getAck()<<"|"<<std::bitset<32>(p.getAcksRecent())<<"|"
			 <<"|"<<p.getSize()<<"|";
}


