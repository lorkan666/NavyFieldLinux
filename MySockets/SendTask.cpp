/*
 * SendTask.cpp
 *
 *  Created on: 4 wrz 2015
 *      Author: lorkan
 */

#include "SendTask.h"

SendTask::SendTask(MyPacket p, Address adr) {
	this->p = p;
	this->address = adr;
}
