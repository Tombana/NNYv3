#ifndef H_PACKETDISPATCHER
#define H_PACKETDISPATCHER

//Relative `Common` and `Resource` files
#include "resTypedef.h" //define CMD type
#include "Packet.h"
//Define SESSION type
#include "session.h"
//All packet handlers for dispatching
#include "Packets/ConnectionPackets.h"
#include "Packets/CorePackets.h"
#include "Packets/RealmPackets.h"

class PacketDispatcher {
public:
	static void dispatch(SESSION& session, Packet& input);
};

#endif
