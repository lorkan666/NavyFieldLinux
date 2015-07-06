#include <iostream>
#include <string>
#include "MySockets.h"
#include "Portier.h"

using namespace std;
const float DeltaTime = 1.0f / 30.0f;

int main()
{
	if(!initSockets()){
		cout<<"Cannot initialize sockets."<<endl;
		return -1;
	}

	try{
		Portier p = Portier();
		cin.get();

	}catch(string &caught){
		cout<<"Got: "<<caught.c_str()<<endl;
	}catch(...){
		cout<<"Got: Unknow Exception"<<endl;
	}
	shutdownSockets();
    return 0;
}
