#include "RealmPackets.h"

//==============================
// PCKT_R_SYNC_KEY_ACK
//==============================
void RealmPackets::syncKeyACK(Packet& input) {
	std::cout << "[RealmPackets][syncKeyACK] PCKT_R_SYNC_KEY_ACK received!" << std::endl;
	if (input.read<ACK>() == ACK_SUCCESS) {
		std::cout << "We are now online!" << std::endl;
	} else {
		std::cout << "The realm explicitly refused our key!" << std::endl;
	}
}

//==============================
// PCKT_R_WELCOME
//==============================
void RealmPackets::welcome(SESSION& session) {
	std::cout << "[RealmPackets][welcome] PCKT_R_WELCOME received!" << std::endl;
	Packet packetToSend;
	if (session->isARealmServer) {
		packetToSend.add<CMD>(PCKT_W_SYNC_KEY);
		packetToSend.add<DWORD>(1); //TODO: Server ID from a configuration file
		packetToSend.addString("7jpWng9Wjc"); //TODO: Same here, no hard-coded server key
	} else {
		packetToSend.add<CMD>(PCKT_X_HACKING_ATTEMPT);
		//TODO: Disconnect the user
	}
	session->socket.write(packetToSend);
}