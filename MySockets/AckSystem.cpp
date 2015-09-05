/*
 * AckSystem.cpp
 *
 *  Created on: 28 cze 2015
 *      Author: lorkan
 */

#include "AckSystem.h"
#include "MyPacket.h"

AckSystem::AckSystem() {
	max_sequence = 0xFFFFFFFF;
	this->rtt_maximum = 1.0f;
	pl = NULL;
	reset();
}

AckSystem::AckSystem(unsigned int max_sequence = 0xFFFFFFFF) {
	this->max_sequence=max_sequence;
	reset();
}

AckSystem::~AckSystem() {
}

unsigned int AckSystem::generateAckBits() {
	unsigned int ack_bits = 0;
	for ( MyPacketList::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++ )
	{
		if ( itor->getSequence() == remote_sequence || isSequenceRecent( itor->getSequence(), remote_sequence, max_sequence ) )
			continue;
		int bit_index = bitIndexAck( itor->getSequence(), remote_sequence, max_sequence );
		if ( bit_index <= 31 )
			ack_bits |= 1 << bit_index;
	}
	return ack_bits;
}

int AckSystem::bitIndexAck(unsigned int sequence, unsigned int ack,unsigned int max_sequence) {
	if(sequence == ack || isSequenceRecent(sequence, ack, max_sequence))
		return 0; //asercja
	if ( sequence > ack )
	{
		if(ack >=33 || max_sequence < sequence)
			return 0; //asercja
		return ack + ( max_sequence - sequence );
	}
	else
	{
		if(ack <1 || sequence > (ack - 1))
					return 0; //asercja
		return ack - 1 - sequence;
	}
}

void AckSystem::processAck(MyPacket p) {
	if ( pendingAckQueue.empty() )
		return;

	MyPacketList::iterator itor = pendingAckQueue.begin();
	while ( itor != pendingAckQueue.end()){
		bool acked = false;

		if(itor->getSequence() == p.getAck()){
			acked = true;
		}
		else if (!isSequenceRecent( itor->getSequence(), p.getAck(), max_sequence ) ){
			int bit_index = bitIndexAck( itor->getSequence(), p.getAck(), max_sequence );
			if ( bit_index <= 31 )
				acked = ( p.getAcksRecent() >> bit_index ) & 1;
		}

		if(acked){
			rtt += ( itor->time - rtt ) * 0.1f;

			ackedQueue.insert_sorted( *itor, max_sequence );
			acks.push_back( itor->getSequence() );
			acked_packets++;
			itor->delivered = true;
			if(pl != NULL){
				pl->onPacketDelivered(*itor,address);
			}
			itor = pendingAckQueue.erase( itor );
		}
		else{
			++itor;
		}
	}
}

void AckSystem::update(float deltaTime) {
	acks.clear();
	advanceQueueTime( deltaTime );
	updateQueues();
	updateStats();
}

void AckSystem::reset() {
	local_sequence = 0;
	remote_sequence = 0;
	sentQueue.clear();
	receivedQueue.clear();
	pendingAckQueue.clear();
	ackedQueue.clear();
	sent_packets = 0;
	recv_packets = 0;
	lost_packets = 0;
	acked_packets = 0;
	sent_bandwidth = 0.0f;
	acked_bandwidth = 0.0f;
	rtt = 0.0f;
	rtt_maximum = 1.0f;
}

void AckSystem::packetSent( MyPacket p )
{
	if(sentQueue.exists(local_sequence) || pendingAckQueue.exists( local_sequence )){
		return; //asercja
	}
	p.time = 0;
	sentQueue.push_back(p);
	pendingAckQueue.push_back(p);
	sent_packets++;
	local_sequence+=1;
	if ( local_sequence > max_sequence )
		local_sequence = 0;
}

void AckSystem::packetReceived( MyPacket p )
{
	recv_packets++;
	if ( receivedQueue.exists(p.getSequence()))
		return;

	p.time = 0;
	receivedQueue.push_back(p);
	if ( isSequenceRecent(p.getSequence(), remote_sequence, max_sequence )){
		remote_sequence = p.getSequence();
	}
}

void AckSystem::advanceQueueTime(float deltaTime) {
	for ( MyPacketList::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++ )
		itor->time += deltaTime;

	for ( MyPacketList::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++ )
		itor->time += deltaTime;

	for ( MyPacketList::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++ )
		itor->time += deltaTime;

	for ( MyPacketList::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++ )
		itor->time += deltaTime;
}

void AckSystem::updateQueues() {
	const float epsilon = 0.001f;

	while ( sentQueue.size() && sentQueue.front().time > rtt_maximum + epsilon )
		sentQueue.pop_front();

	if ( receivedQueue.size() )
	{
		const unsigned int latest_sequence = receivedQueue.back().getSequence();
		const unsigned int minimum_sequence = latest_sequence >= 34 ? ( latest_sequence - 34 ) : max_sequence - ( 34 - latest_sequence );
		while ( receivedQueue.size() && !isSequenceRecent( receivedQueue.front().getSequence(), minimum_sequence, max_sequence ) )
			receivedQueue.pop_front();
	}

	while ( ackedQueue.size() && ackedQueue.front().time > rtt_maximum * 2 - epsilon )
		ackedQueue.pop_front();

	while ( pendingAckQueue.size() && pendingAckQueue.front().time > rtt_maximum + epsilon )
	{
		MyPacket p = *(pendingAckQueue.begin());
		pendingAckQueue.pop_front();
		lost_packets++;
		if(pl != NULL){
			pl->onPacketLost(p,address);
		}
	}
}

void AckSystem::updateStats() {
	int sent_bytes_per_second = 0;
	for ( MyPacketList::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor )
		sent_bytes_per_second += itor->getSize();
	int acked_packets_per_second = 0;
	int acked_bytes_per_second = 0;
	for ( MyPacketList::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor )
	{
		if ( itor->time >= rtt_maximum )
		{
			acked_packets_per_second++;
			acked_bytes_per_second += itor->getSize();
		}
	}
	sent_bytes_per_second /= rtt_maximum;
	acked_bytes_per_second /= rtt_maximum;
	sent_bandwidth = sent_bytes_per_second * ( 8 / 1000.0f );
	acked_bandwidth = acked_bytes_per_second * ( 8 / 1000.0f );
}

void AckSystem::setPacketListener(PacketListener* pl) {
	this->pl=pl;
}

void AckSystem::setRTTmaximum(float rtt_max) {
	this->rtt_maximum = rtt_max;
}
