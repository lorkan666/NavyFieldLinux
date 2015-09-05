/*
 * SendTask.h
 *
 *  Created on: 4 wrz 2015
 *      Author: lorkan
 */

#ifndef SENDTASK_H_
#define SENDTASK_H_
#include "MyPacket.h"
#include "Address.h"

class SendTask {
public:
	MyPacket p;
	Address address;

	SendTask(MyPacket p,Address adr);
};

#endif /* SENDTASK_H_ */
