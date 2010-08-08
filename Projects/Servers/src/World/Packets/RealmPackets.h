#ifndef H_REALM_PACKETS
#define H_REALM_PACKETS

//STD library
#include <iostream>
//Relative `Commons` and `Resources`
#include "resTypedef.h"
#include "resProtocol.h"
//Include session definition
#include "../session.h"

class RealmPackets {
public:
	static void welcome(SESSION& session);
	static void syncKeyACK(Packet& input);
};

#endif