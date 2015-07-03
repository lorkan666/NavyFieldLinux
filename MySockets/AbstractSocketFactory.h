/*
 * ssss.h
 *
 *  Created on: 23 cze 2015
 *      Author: lorkan
 */

#ifndef AbstractSocketFactory_H_
#define AbstractSocketFactory_H_
#include "AbstractSocket.h"

class AbstractSocketFactory {
public:
	virtual AbstractSocket * createSocket(const char* adress, unsigned short port)=0;
};

#endif /* AbstractSocketFactory_H_ */
