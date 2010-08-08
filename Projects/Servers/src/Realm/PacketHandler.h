#ifndef H_PACKETHANDLER
#define H_PACKETHANDLER

#include <iostream>
#include <string>
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "SessionMgr.h"
#include "PacketDispatcher.h"
#include "WorldlinkMgr.h"
#include "Socket.h"

//Some typedefs needed for the implementation code
#include "session.h" //define s_session
typedef ACE_Singleton<SessionMgr<SESSION>,ACE_Null_Mutex> SESSIONMGR;

#define BUFFER_SIZE 1024

/**
* @class PacketHandler
* @brief Handle connection and packet events
* 
* PacketHandler is a derivated class of ACE service handlers (@a ACE_Svc_Handler).\n
* With ACE, programs are designed such as you can have as many service handlers as you need, connecting them to want you want to handle of course.
* Here I chose to connect this class to @c ACE_Acceptor wich mostly deals with new connection & packet and generate appropriate events from them.
* @note You can connect many ACE modules to the same service handler but in our case it would be really messy.
* @see The connection is made in @a main()
**/
class PacketHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM,ACE_NULL_SYNCH> { 
public:
	/**
	* This function is called from @c ACE_Acceptor on every new connections. The implementation also create a new session in memory with SessionMgr.
	* @brief Called on every new connections.
	* @note It doesn't have the handle in the parameters so you might want to use @a get_handle() here.
	* @param[in] void* ACE wants a void pointer here, don't ask me.
	* @return We always return 0 to tell the @c ACE_Reactor to keep the connection between this service handle and @c ACE_Acceptor.
	**/
	int open(void*);
	/**
	* This function is called from @c ACE_Acceptor on every new packets.\n
	* Packet's are buffered and once enough bytes are accumulated to make a capsule, this capsule is sent to the PacketDispatcher.\n
	* @brief Called on every new packets.
	* @note It would be impossible to save permanently the packet buffer in memory here, that's why we are using a session manager (SessionMgr) to solve the problem.
	* @param[in] handle is used by the SessionMgr to return a session pointer (@a SESSION), wich is then used to acces the user's packet buffer.
	* @return 	We return 0 to stay registered with the @c ACE_Reactor
	*				@b OR
	* 			We return -1 to call close_handle() and close the socket.
	**/
	int handle_input(ACE_HANDLE handle);
};

#endif