#include "CapsuleRealmSyncKeyAck.h"

void CapsuleRealmSyncKeyAck::doit(s_session* session, Packet& capsule) {
	std::cout << "[CapsuleRealmSyncKeyAck] Packet PCKT_R_SYNC_KEY_ACK received!" << std::endl;
	if (capsule.read<ACK>() == ACK_SUCCESS) {
		std::cout << "We are now online!" << std::endl;
	} else {
		std::cout << "The realm explicitly refused our key!" << std::endl;
	}
}