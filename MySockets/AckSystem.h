/*
 * AckSystem.h
 *
 *  Created on: 28 cze 2015
 *      Author: lorkan
 */

#ifndef ACKSYSTEM_H_
#define ACKSYSTEM_H_
#include <list>
#include <vector>
#include <iostream>
#include "MyPacket.h"
#include "PacketListener.h"

class AckSystem {

public:
	AckSystem();
	AckSystem(unsigned int max_sequence);
	Address address;
	void packetSent(MyPacket p );
	void packetReceived(MyPacket p );
	void setPacketListener(PacketListener * pl);
	void setRTTmaximum(float rtt_max);
	unsigned int generateAckBits();
	void processAck(MyPacket p);
	void update( float deltaTime );
	int bitIndexAck( unsigned int sequence, unsigned int ack, unsigned int max_sequence );
	float rtt_maximum;					// maximum expected round trip time (hard coded to one second for the moment)

	virtual ~AckSystem();
	class MyPacketList: public std::list<MyPacket> {
	public:
		bool exists( unsigned int sequence )
		{
			for ( iterator itor = begin(); itor != end(); ++itor ){
				if ( itor->getSequence() == sequence ){
					return true;
				}
			}
			return false;
		}

		void insert_sorted(MyPacket & p, unsigned int max_sequence )
		{
			if(empty()){
				push_back( p );
			}
			else{
				if(!isSequenceRecent( p.getSequence(), front().getSequence(), max_sequence ) )
				{
					push_front( p );
				}
				else if(isSequenceRecent( p.getSequence(), back().getSequence(), max_sequence ) )
				{
					push_back( p );
				}
				else{
					for ( MyPacketList::iterator itor = begin(); itor != end(); itor++ ){
						if(itor->getSequence() == p.getSequence() )
							return; //asercja
						if(isSequenceRecent(itor->getSequence(), p.getSequence(), max_sequence )){
							insert( itor, p );
							break;
						}
					}
				}
			}
		}

		void verify_sorted( unsigned int max_sequence )
		{
			MyPacketList::iterator prev = end();
			for ( MyPacketList::iterator itor = begin(); itor != end(); itor++ )
			{
				if( itor->getSequence() > max_sequence ){
					return; //asercja
				}
				if ( prev != end() )
				{
					if( !isSequenceRecent( itor->getSequence(), prev->getSequence(), max_sequence ))
						return; //asercja
					prev = itor;
				}
			}
		}
	};

	void reset();

	unsigned int getLocalSequence() const {
		return local_sequence;
	}

	unsigned int getRemoteSequence() const {
		return remote_sequence;
	}

	float getAckedBandwidth() const {
		return acked_bandwidth;
	}

	unsigned int getAckedPackets() const {
		return acked_packets;
	}

	unsigned int getLostPackets() const {
		return lost_packets;
	}

	unsigned int getRecvPackets() const {
		return recv_packets;
	}

	float getRtt() const {
		return rtt;
	}

	float getSentBandwidth() const {
		return sent_bandwidth;
	}

	unsigned int getSentPackets() const {
		return sent_packets;
	}

private:
	PacketListener * pl;
	void advanceQueueTime( float deltaTime );
	void updateQueues();
	void updateStats();

	unsigned int max_sequence;			// maximum sequence value before wrap around (used to test sequence wrap at low # values)
	unsigned int local_sequence;		// local sequence number for most recently sent packet
	unsigned int remote_sequence;		// remote sequence number for most recently received packet

	unsigned int sent_packets;			// total number of packets sent
	unsigned int recv_packets;			// total number of packets received
	unsigned int lost_packets;			// total number of packets lost
	unsigned int acked_packets;			// total number of packets acked

	float sent_bandwidth;				// approximate sent bandwidth over the last second
	float acked_bandwidth;				// approximate acked bandwidth over the last second
	float rtt;							// estimated round trip time


	std::vector<unsigned int> acks;		// acked packets from last set of packet receives. cleared each update!

	MyPacketList sentQueue;				// sent packets used to calculate sent bandwidth (kept until rtt_maximum)
	MyPacketList pendingAckQueue;		// sent packets which have not been acked yet (kept until rtt_maximum * 2 )
	MyPacketList receivedQueue;			// received packets for determining acks to send (kept up to most recent recv sequence - 32)
	MyPacketList ackedQueue;
};

#endif /* ACKSYSTEM_H_ */
