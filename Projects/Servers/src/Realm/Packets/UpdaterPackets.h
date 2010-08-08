#ifndef H_UPDATERPACKETS
#define H_UPDATERPACKETS

//STD library
#include <iostream>
//Relative `Commons` and `Resources` files
#include "resProtocol.h"
#include "resTypedef.h"
#include "database.h"
#include "Packet.h"
//Parent folder includes
#include "../session.h"

extern database::connection g_db;
extern unsigned int	g_client_version;

class UpdaterPackets {
public:
	static void checkVersion(SESSION& session, Packet& input);
};

#endif