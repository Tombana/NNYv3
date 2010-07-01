#include "CapsuleDebug.h"

void CapsuleDebug::doit(s_session* session, Packet& capsule) {
		std::cout << "[CapsuleDebug] Packet PCKT_X_DEBUG received!" << std::endl;
		std::cout << "Message: " << capsule.readString() << std::endl;
}