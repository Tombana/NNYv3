#include "CapsuleWorldlist.h"

void CapsuleWorldlist::doit(s_session* session, Packet& capsule) {
	std::cout << "[CapsuleWorldlist] PCKT_C_GETWORLDLIST received!" << std::endl;
	session->socket.write(WORLDLINKMGR::instance()->getGeneratedPacket());
	session->socket.close();
}