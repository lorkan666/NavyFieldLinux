#include <iostream>
#include <string>
#include "MySockets.h"

using namespace std;
const float DeltaTime = 1.0f / 30.0f;

int main()
{
	if(!initSockets()){
		cout<<"Cannot initialize sockets."<<endl;
		return -1;
	}

	try{
		MyUDPConnection connection = MyUDPConnection(0xFFAAAAFF);

		connection.setTimeout(10);
		connection.keepAlive(true);
		connection.setMode(ConnectionInfo::Server);
		//connection.setPacketListener(new JakisPacketListener());
		connection.start(Address(0,30000));

		cin.get();

		connection.stop();

	}catch(string &caught){
		cout<<"Got: "<<caught.c_str()<<endl;
	}catch(...){
		cout<<"Got: Unknow Exception"<<endl;
	}
	shutdownSockets();
    return 0;
}
