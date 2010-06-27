#ifndef H_MAIN
#define H_MAIN

//STD library includes
#include <iostream>
#include <cstdlib> //EXIT_SUCCESS & EXIT_FAILURE

//Handling signals
#include <signal.h>

//ACE library includes
#include "ace/Thread.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Reactor.h"

//Common resource files
#include "resTypedef.h"
#include "resProtocol.h"

//Services handlers
#include "PacketHandler.h"

//Namespaces
#include "database.h"

//Configuration file
#include "config.h"

//TODO: testing
#include "WorldlinkMgr.h"

//Functions
void handle_signal(int signal);

#endif
