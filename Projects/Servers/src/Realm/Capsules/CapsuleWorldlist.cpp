#include "CapsuleWorldlist.h"

void CapsuleWorldlist::doit(SESSION session, Packet& capsule) {
	std::cout << "[CapsuleWorldlist] PCKT_C_GETWORLDLIST received!" << std::endl;
	session->socket.write(WORLDLINKMGR::instance()->getGeneratedPacket());
}