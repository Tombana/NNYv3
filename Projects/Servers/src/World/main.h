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

//RealmConnector
#include "RealmConnector.h"

//Functions
void handle_signal(int signal);

#endif
