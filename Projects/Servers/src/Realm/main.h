#ifndef H_MAIN
#define H_MAIN

//STD library includes
#include <iostream>
#include <cstdlib> //EXIT_SUCCESS & EXIT_FAILURE
#include <signal.h> //Handling signals
//ACE library includes
#include "ace/OS.h"
#include "ace/Time_Value.h"
#include "ace/Thread.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Reactor.h"
//Resource files
#include "resTypedef.h"
#include "resProtocol.h"
//Common files
#include "database.h"
#include "PacketHandler.h"
#include "PacketDispatcher.h"
//Configuration file
#include "config.h"
//Needed to setup WorldlinkMgr's instance
#include "WorldlinkMgr.h"

//Some typedefs needed to setup PacketDispatcher's instance
#include "session.h" //define s_session
typedef	s_session* SESSION;

//Functions
void handle_signal(int signal);

#endif
