/*
 * MySockets.h
 *
 *  Created on: 24 cze 2015
 *      Author: lorkan
 */

#ifndef MYSOCKETS_H_
#define MYSOCKETS_H_

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

	#include <winsock2.h>
	#pragma comment( lib, "wsock32.lib" )

	#define mysleep( ms ) Sleep(ms);

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <fcntl.h>

	#define mysleep( ms ) usleep( (int) ( ms * 1000.0f ) );

#endif

#include "Address.h"
#include "AbstractSocket.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "AbstractSocketFactory.h"
#include "SocketFactories.h"
#include "MyPacket.h"
#include "MyUDPConnection.h"

inline bool initSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData;
		return WSAStartup( MAKEWORD(2,2), &WsaData ) != NO_ERROR;
	#else
		return true;
	#endif
}

inline void shutdownSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
	#endif
}

#endif /* MYSOCKETS_H_ */
