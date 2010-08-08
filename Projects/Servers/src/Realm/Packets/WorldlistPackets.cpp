#include "WorldlistPackets.h"

void WorldlistPackets::getWorldlist(SESSION& session) {
	std::cout << "[WorldlistPacket][getWorldlist] PCKT_C_GETWORLDLIST received!" << std::endl;
	session->socket.write(WORLDLINKMGR::instance()->getGeneratedPacket());
	session->socket.close();
}