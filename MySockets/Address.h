/*
 * Address.h
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#ifndef ADDRESS_H_
#define ADDRESS_H_
#include <iostream>
using namespace std;

class Address
{
public:
    Address();
    Address( unsigned char a,
             unsigned char b,
             unsigned char c,
             unsigned char d,
             unsigned short port );
    Address( unsigned int address,
             unsigned short port );
    Address( string adr,  unsigned short port );
    Address(const Address& adr);
    unsigned int GetAddress() const;
    unsigned short GetPort() const;
    bool operator == ( const Address & other ) const;
    bool operator != ( const Address & other ) const;
    bool operator < ( const Address & other ) const;

private:

    unsigned int address;
    unsigned short port;
};

extern std::ostream & operator <<( std::ostream & s, Address & p );

#endif /* ADDRESS_H_ */
