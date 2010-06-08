#ifndef H_PACKETHANDLER
#define H_PACKETHANDLER

#include <iostream>
#include <string>
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "SessionMgr.h"
#include "PacketDispatcher.h"
#include "Socket.h"

#define BUFFER_SIZE 1024

//Service handler for packets
class PacketHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH> { 
public:
	PacketHandler();
	~PacketHandler();
	int open(void*);
	int handle_input(ACE_HANDLE);
};

#endif