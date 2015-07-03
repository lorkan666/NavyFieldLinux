/*
 * Address.cpp
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#include "Address.h"
#include "MySockets.h"

Address::Address()
{
    address = 0;
    port = 0;
}

Address::Address(unsigned char a, unsigned char b, unsigned char c,	unsigned char d, unsigned short port) {
	this->address = ( a << 24 ) | ( b << 16 ) | ( c << 8  ) |  d;
	this->port = port;
}

Address::Address(unsigned int address, unsigned short port) {
	this->address = address;
	this->port = port;
}

Address::Address( const Address & adr )
{
    address = adr.address;
    port = adr.port;
}

unsigned int Address::GetAddress() const {
	return address;
}

unsigned short Address::GetPort() const {
	return port;
}

bool Address::operator == ( const Address & other ) const
{
	return address == other.address && port == other.port;
}

bool Address::operator != ( const Address & other ) const
{
	return ! ( *this == other );
}

bool Address::operator < ( const Address & other ) const
{
	if ( address < other.address )
		return true;
	if ( address > other.address )
		return false;
	else
		return port < other.port;
}

Address::Address(string adr, unsigned short port) {
	address = ntohl(inet_addr( adr.c_str()));
	this->port = port;
}
