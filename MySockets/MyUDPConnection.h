/*
 * MyUDPConnection.h
 *
 *  Created on: 25 cze 2015
 *      Author: lorkan
 */

#ifndef MYUDPCONNECTION_H_
#define MYUDPCONNECTION_H_
#include "UDPSocket.h"
#include "MyPacket.h"
#include "ConnectionInfo.h"
#include "ConnectionInfoList.h"
#include "ConnectionListener.h"
#include "PacketListener.h"
#include "SendTask.h"
#include <thread>
#include <map>
#include <list>


using namespace std;

class Address;
class MyUDPConnection : public PacketListener{
public:

	MyUDPConnection();
	MyUDPConnection(unsigned int protocol_id);

	ConnectionInfo::State getState(Address address);

	bool isConnected();
	void display(string s);
	void keepAlive(bool a);
	bool start(Address address);
	void stop();
	virtual void onPacketSent(MyPacket p, Address address);
	virtual void onPacketNoneSent(MyPacket p, Address address);
	virtual void onPacketReceived(MyPacket p, Address address);
	virtual void onPacketDelivered(MyPacket p, Address address);
	virtual void onPacketLost(MyPacket p, Address address);
	bool isConnected(Address &address);
	void setPacketListener(PacketListener * pl);
	void setConnectionListener(ConnectionListener * cl);
	void setMode(ConnectionInfo::Mode m);
	void setRttMax(float rtt);
	static void *connectionThreadFunction(void *params);
	virtual bool sendPacket(MyPacket &p, Address & address);
	bool sendTask(MyPacket &p, Address & address);
	virtual int receivePacket(MyPacket &p);
	virtual void update( float delta);
	void removeDisconnected();
	void connect( const Address & address );
	void disconnect( Address address );
	float getTimeout() const;
	void setTimeout(float timeout);
	virtual ~MyUDPConnection();

protected:
	ConnectionInfoList virtual_connections;
	unsigned int protocol_id;
	bool keeping_alive;
	list<SendTask> toSend;

private:
	PacketListener * pl;
	ConnectionListener * cl;
	float DeltaTime;
	UDPSocket socket;
	bool connection_thread_work;
	ConnectionInfo info;
	void setState(ConnectionInfo::State s);
	float timeout;
	std::thread * connection_thread;
	bool startConnectionThread();
	void stopConnectionThread();
};

#endif /* MYUDPCONNECTION_H_ */
