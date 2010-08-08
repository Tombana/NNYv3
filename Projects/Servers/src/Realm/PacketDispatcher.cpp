#include "PacketDispatcher.h"

void PacketDispatcher::dispatch(SESSION& session, Packet& input) {
	//I used to deal with arrays of function pointers but I just learned the compiler will optimize
	//this switch to be as fast as possible. The compiler is always right and if you think you can do
	//better than theses guys, I'd really suggest you to quit every projects you are working on,
	//share your new discovery and get yourself known.
	//Here we are aiming for a switch jump table but again it's according to your compiler capability.
	//Please note the cases must be as narrow as possible with only few gasp,
	//otherwise the compiler might consider the trade-off and use another method, such as:
	//Binary search, any fancy tree or simply bunch of if/elseif/else statements.
	switch (input.read<CMD>()) {	//TODO: Sort this switch case.
		case PCKT_X_DEBUG:			CorePackets::debug(input); break;
		case PCKT_C_GETWORLDLIST:	WorldlistPackets::getWorldlist(session); break;
		case PCKT_C_VERSION:		UpdaterPackets::checkVersion(session, input); break;
		case PCKT_W_SYNC_KEY:		WorldPackets::syncKey(session, input); break;
		default:					CorePackets::invalidCMD(input);
	}
}