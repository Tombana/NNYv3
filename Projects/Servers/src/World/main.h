#ifndef H_MAIN
#define H_MAIN

//STD library includes
#include <iostream>

//ACE library includes
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Reactor.h"

//Common resource files
//#include "resTypedef.h"
#include "resProtocol.h"

//Things the server needs
#include "PacketHandler.h"
#include "database.h"
#include "config.h"

//Needed to add sources (capsules) to the dispatcher
#include "Capsules/CapsuleDebug.h"
#include "Capsules/CapsuleRealmWelcome.h"
#include "Capsules/CapsuleRealmSyncKeyAck.h"

//Setting up the dispatcher
#include "PacketDispatcher.h"
//Some typedefs for the implementation
#include "session.h" //define s_session
typedef	s_session* SESSION;
typedef ACE_Singleton<SessionMgr<SESSION>,ACE_Null_Mutex> SESSIONMGR;
typedef ACE_Singleton<PacketDispatcher<SESSION>,ACE_Null_Mutex> PACKETDISPATCHER;

//RealmConnector
#include "RealmConnector.h"

//Functions
void handle_signal(int signal);

#endif
