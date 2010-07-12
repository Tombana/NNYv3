#include "CapsuleRealmWelcome.h"

void CapsuleRealmWelcome::doit(s_session* session, Packet& capsule) {
	std::cout << "[CapsuleRealmWelcome] Packet PCKT_R_WELCOME received!" << std::endl;
	Packet packetToSend;
	packetToSend.add<CMD>(PCKT_W_SYNC_KEY);
	packetToSend.add<DWORD>(1); //TODO: SERVER ID
	packetToSend.addString("7jpWng9Wjc"); //TODO: SERVER KEY
	session->socket.write(packetToSend);
}