#include "WorldPackets.h"

void WorldPackets::syncKey(SESSION& session, Packet& input) {
	std::cout << "[WorldPackets][syncKey] PCKT_W_SYNC_KEY received!" << std::endl;
	//Save vars
	DWORD serverID = input.read<DWORD>();
	std::string sync_key = input.readString();
	std::cout << "Server ID: " << serverID << std::endl;
	std::cout << "Sync key: " << sync_key << std::endl;
	//Get the saved sync_key from WORDLINKMGR
	WorldlinkMgr::s_link *link = WORLDLINKMGR::instance()->getLink(serverID);
	//Prepare a packet
	Packet packetToSend;
	packetToSend.add<CMD>(PCKT_R_SYNC_KEY_ACK);
	//Perform a check
	if (link) { //check if a link was found
		if (link->sync_key == sync_key) {
			packetToSend.add<ACK>(ACK_SUCCESS);
			WORLDLINKMGR::instance()->createLink(serverID);
			session->worldID = serverID;
			session->isAWorldServer = true;
			link->socket = session->socket;
		} else {
			packetToSend.add<ACK>(ACK_FAILURE);
		}
	} else {
		packetToSend.add<ACK>(ACK_FAILURE);
	}
	//Send packet
	session->socket.write(packetToSend);
}