/*
 * MyPacket.h
 *
 *  Created on: 25 cze 2015
 *      Author: lorkan
 */

#ifndef MYPACKET_H_
#define MYPACKET_H_
#include <iostream>
#include <cstring>
#include <bitset>
using namespace std;

#define MTU 500

typedef struct {
	unsigned int pid;
	unsigned int sequence;
	unsigned int ack;
	unsigned int acks_recent;
} PacketHeader;

inline bool isSequenceRecent( unsigned int s1, unsigned int s2, unsigned int max_sequence )
{
	return (( s1 > s2 ) && ( (s1 - s2) <= max_sequence/2 )) || (( s2 > s1 ) && ( (s2 - s1) > (max_sequence/2) ));
}

class MyPacket {
public:
	MyPacket();
	MyPacket(const char * data, unsigned int size);
	MyPacket(const MyPacket& coped);
	virtual ~MyPacket();
	bool isDelivered();
	static void writeInt( unsigned char * data, unsigned int value );
	static void writeInt( unsigned int * data, unsigned int value );
	static unsigned int readInt( char * data);
	static unsigned int readInt( unsigned int * data);
	unsigned int getAck() ;
	void setAck(unsigned int ack);
	unsigned int getAcksRecent() ;
	void setAcksRecent(unsigned int acksRecent);
	unsigned int getPid();
	void setPid(unsigned int pid);
	unsigned int getSequence() ;
	void setSequence(unsigned int sequence);
	const char* getData();
	int getSize();
	void setSize(int size);
	int getMaxSize();
	int getHeaderSize();
	void setRetransmiting(int r);
	int getRetransmiting();
	float time;
	char * getDataPointer();
	bool delivered;
	bool operator ==( const MyPacket & v );
	MyPacket& operator=(const MyPacket& p) ;

protected:
	PacketHeader * header;
	char * data;
	int size;
	int retransmiting;
};


extern std::ostream & operator <<( std::ostream & s, MyPacket & p );

#endif /* MYPACKET_H_ */
