#ifndef H_CONNECTION_PACKETS
#define H_CONNECTION_PACKETS

//STD library
#include <iostream>
#include <string>
#include <algorithm>
//Relative `Common` & `Resource` files
#include "Packet.h"
#include "database.h"
#include "config.h"
#include "resProtocol.h"
#include "resTypedef.h"
//Parent folder
#include "../session.h"

extern database::connection g_db;

class ConnectionPackets {
public:
	static void auth(SESSION& session, Packet& input);
};

#endif