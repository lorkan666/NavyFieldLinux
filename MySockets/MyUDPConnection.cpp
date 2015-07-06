/*
 * MyUDPConnection.cpp
 *
 *  Created on: 25 cze 2015
 *      Author: lorkan
 */

#include "MyUDPConnection.h"
#include "ConnectionInfo.h"
#include "Address.h"
#include "MySockets.h"
#include <iostream>
#include <thread>
#include <ctime>
#include <errno.h>

//#pragma comment (lib, "pthread.lib")
using namespace std;

#define keeping_alive_ratio 0.3  //send interval = timeout * keeping_alive_ratio

MyUDPConnection::MyUDPConnection() {
	timeout = 10.0f;
	protocol_id = 0xFFFFFFFF;
	info.mode = ConnectionInfo::None;
	info.last_send_time = time(NULL);
	keeping_alive = false;
	connection_thread = NULL;
	pl=NULL;
	cl=NULL;
	connection_thread_work = false;
	DeltaTime = 1.0f / 30.0f;
}

MyUDPConnection::MyUDPConnection(unsigned int protocol_id):MyUDPConnection() {
	this->protocol_id=protocol_id;
}

bool MyUDPConnection::start(Address address) {
	socket.setPort(address.GetPort());
	socket.setAddress(address.GetAddress());
	if(!socket.openSocket()){
		return false;
	}
	if(!socket.bindSocket()){
		return false;
	}
	if(!socket.setBlocking(false)){
		return false;
	}
	cout<<"Socket opened at "<<address.GetAddress()<<" : "<<address.GetPort()<<
			endl<<"Protocol ID: "<<this->protocol_id<<endl;
	info.state = ConnectionInfo::Disconnected;
	return startConnectionThread();
}

void MyUDPConnection::stop() {
	stopConnectionThread();
	list<ConnectionInfo>::iterator itor;
	for ( itor=virtual_connections.begin();itor != virtual_connections.end();itor++ ){
		itor->setState(ConnectionInfo::Disconnected);
	}
	setState(ConnectionInfo::Disconnected);
	virtual_connections.clear();
	socket.closeSocket();
}


void MyUDPConnection::connect(const Address& address) {
	if(info.mode == ConnectionInfo::Server)
		return;
	if(virtual_connections.contains(address))
		return;
	ConnectionInfo  new_connection =  ConnectionInfo();
	if(cl != NULL)
		new_connection.setConnectionListener(cl);
	new_connection.address = address;
	new_connection.keeping_alive = this->keeping_alive;
	new_connection.mode = ConnectionInfo::Server;
	new_connection.last_send_time = 0;
	new_connection.acks_sys.setPacketListener(this);
	new_connection.setState(ConnectionInfo::Connecting);
	virtual_connections.push_back(new_connection);
}

bool MyUDPConnection::isConnected(Address& address) {
	if(! virtual_connections.contains(address))
		return false;
	return (virtual_connections.get(address)->state == ConnectionInfo::Connected);
}

void MyUDPConnection::disconnect(Address address) {
	if(!virtual_connections.contains(address))
		return;
	ConnectionInfo *ci = virtual_connections.get(address);
	ci->setState(ConnectionInfo::Disconnected);
	cout<<"virtual_connections: "<<virtual_connections.size()<<endl;
	virtual_connections.remove(address);
	cout<<"virtual_connections: "<<virtual_connections.size()<<endl;
}


ConnectionInfo::State MyUDPConnection::getState(Address address) {
	ConnectionInfo *ci = virtual_connections.get(address);
	if(ci == NULL)
		return ConnectionInfo::Disconnected;
	return ci->state;
}

void MyUDPConnection::update(float delta) {
	list<ConnectionInfo>::iterator itor;
	MyPacket p = MyPacket();
	ConnectionInfo *ci;

	for ( itor=virtual_connections.begin();itor != virtual_connections.end();itor++ ){
		ci=&(*itor);
		ci->timeoutAccumulator += delta;
		if((ci->state == ConnectionInfo::Connected)  || (ci->state == ConnectionInfo::Connecting)){
			if (ci->timeoutAccumulator > timeout){
				if ( ci->state == ConnectionInfo::Connecting ){
					ci->setState(ConnectionInfo::ConnectFail);
					continue;
				}
				ci->setState(ConnectionInfo::Disconnected);
				continue;
			}
			if(ci->keeping_alive){

				time_t now = time(NULL);

				if((now - ci->last_send_time) >= (int)(timeout*keeping_alive_ratio))
					if(sendPacket(p,ci->address)){
						ci->last_send_time = now;
					}
			}
			ci->acks_sys.update(delta);
		}
	}
}

void MyUDPConnection::setState(ConnectionInfo::State s) {
	info.setState(s);
}

bool MyUDPConnection::sendPacket(MyPacket &p, Address & address) {
	ConnectionInfo *ci = virtual_connections.get(address);

	p.setPid(this->protocol_id);
	p.setSequence(ci->acks_sys.getLocalSequence());
	p.setAck(ci->acks_sys.getRemoteSequence());
	p.setAcksRecent(ci->acks_sys.generateAckBits());
	if ( !socket.sendData(address,p.getData(),p.getSize())){
		onPacketNoneSent(p,address);
		return false;
	}
	cout<<"Wyslano: "<<p<<endl;
	ci->acks_sys.packetSent(p);

	onPacketSent(p,address);

	return true;
}

int MyUDPConnection::receivePacket(MyPacket& rp) {
	Address sender;
	MyPacket p = MyPacket();
	int bytes_read = socket.receiveData(sender,p.getData(),p.getMaxSize());
	if(bytes_read < p.getHeaderSize())
		return 0;

	if(p.getPid() != protocol_id)
		return 0;

	p.setSize(bytes_read);
	cout<<"Odebrano: "<<p<<endl;
	if(!virtual_connections.contains(sender)){
		if(info.mode != ConnectionInfo::Client){
			ConnectionInfo  new_connection =  ConnectionInfo();
			new_connection.keeping_alive = this->keeping_alive;
			new_connection.address = sender;
			new_connection.mode = ConnectionInfo::Client;
			new_connection.last_send_time = time(NULL);
			new_connection.timeoutAccumulator = 0;
			new_connection.acks_sys.setPacketListener(this);
			new_connection.acks_sys.packetReceived(p);
			new_connection.acks_sys.processAck(p);
			if(cl != NULL)
				new_connection.setConnectionListener(cl);
			new_connection.setState(ConnectionInfo::Connected);
			virtual_connections.push_back(new_connection);
		}
	}else{
		ConnectionInfo *ci = virtual_connections.get(sender);
		if(cl != NULL)
			ci->setConnectionListener(cl);
		if(ci->state == ConnectionInfo::Connecting )
			ci->setState(ConnectionInfo::Connected);
		ci->timeoutAccumulator = 0.0;
		ci->keeping_alive = this->keeping_alive;
		ci->acks_sys.setPacketListener(this);
		ci->acks_sys.packetReceived(p);
		ci->acks_sys.processAck(p);

	}
	rp=p;
	onPacketReceived(rp,sender);
	return bytes_read - p.getHeaderSize();
}

MyUDPConnection::~MyUDPConnection() {
	if(connection_thread_work)
		this->stop();
	if(pl != NULL)
		delete pl;
	if(cl != NULL)
		delete cl;
}

float MyUDPConnection::getTimeout() const {
	return timeout;
}

void MyUDPConnection::setTimeout(float timeout) {
	this->timeout = timeout;
}

void MyUDPConnection::keepAlive(bool a) {
	this->keeping_alive = a;
}

void* MyUDPConnection::connectionThreadFunction(void* params) {
	MyUDPConnection * con = (MyUDPConnection *)params;
	MyPacket rp = MyPacket();
	while (true){
		con->removeDisconnected();
		while (true){

			int bytes_read = con->receivePacket(rp);
			if ( bytes_read == 0 )
				break;
		}
		con->update(con->DeltaTime );
		mysleep(con->DeltaTime*1000);
	}
}

bool MyUDPConnection::startConnectionThread() {
	stopConnectionThread();
	connection_thread = new std::thread(&MyUDPConnection::connectionThreadFunction,this);
//	if(connection_thread == NULL){
//		connection_thread_work = false;
//		perror("startConnectionThread");
//		return false;
//	}
	connection_thread_work = true;
	return true;
}

void MyUDPConnection::stopConnectionThread() {
	if(connection_thread_work){
		connection_thread->detach();
		//connection_thread->join();
		connection_thread = NULL;
		connection_thread_work = false;
	}
}

void MyUDPConnection::display(string s) {
	cout<<s<<endl;
}

bool MyUDPConnection::isConnected() {
	return (info.state == ConnectionInfo::Connected);
}

void MyUDPConnection::setMode(ConnectionInfo::Mode m) {
	info.mode = m;
}

void MyUDPConnection::removeDisconnected() {
	ConnectionInfo *ci;
	list<ConnectionInfo>::iterator itor;
	for ( itor=virtual_connections.begin();itor != virtual_connections.end(); ){
		ci=&(*itor);
		if ( ci->state == ConnectionInfo::Disconnected || ci->state == ConnectionInfo::ConnectFail )
		{
			itor = virtual_connections.erase(itor);
		}else{
			++itor;
		}
	}
}

void MyUDPConnection::onPacketSent(MyPacket p, Address address) {
	if(pl != NULL)
		pl->onPacketSent(p,address);
}

void MyUDPConnection::onPacketNoneSent(MyPacket p, Address address) {
	if(pl != NULL)
		pl->onPacketNoneSent(p,address);
	cout<<"Nie wysłano: "<<p<<endl;
}

void MyUDPConnection::onPacketReceived(MyPacket p, Address address) {
	if(pl != NULL)
		pl->onPacketReceived(p,address);
}

void MyUDPConnection::onPacketDelivered(MyPacket p, Address address) {
	if(pl != NULL)
		pl->onPacketDelivered(p,address); //dopracowac address nie ten
	cout<<"Dostarczony: "<<p<<endl;
}

void MyUDPConnection::setPacketListener(PacketListener* pl) {
	this->pl=pl;
}

void MyUDPConnection::setConnectionListener(ConnectionListener * cl) {
	this->cl=cl;
}

void MyUDPConnection::onPacketLost(MyPacket p, Address address) {
	if(pl != NULL)
		pl->onPacketLost(p,address); //dopracowac address nie ten
	cout<<"Zgubiony: "<<p<<endl;
}

