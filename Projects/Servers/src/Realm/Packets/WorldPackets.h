//STD library
#include <iostream>
//Relative `Commons` and `Resources` files
#include "Packet.h"
#include "resTypedef.h"
#include "resProtocol.h"
//Parent folder includes
#include "../WorldlinkMgr.h"
#include "../session.h"

class WorldPackets {
public:
	static void syncKey(SESSION& session, Packet& input);
};
