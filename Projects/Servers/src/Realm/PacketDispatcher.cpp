#include "PacketDispatcher.h"

PacketDispatcher::PacketDispatcher() {
	//Set the whole array set to NULL; just in case.
	//Note that it's impossible to initialize an array 
	//in a class's initialization list or as static/const.
	for(unsigned int i=0; i<NNY_PROTOCOL_RANGE; i++)
		m_table[i] = NULL;
	//Now fill the table with our packets of interest
	//================== TABLE OF INTEREST =======================
	m_table[PCKT_C_REVISION]		= new CapsuleRevision;
	m_table[PCKT_X_DEBUG]			= new CapsuleDebug;
	//============================================================
}
PacketDispatcher::~PacketDispatcher() {
	//Free memory of array objects; note delete NULL won't crash
	//If the argument is 0, the delete expression has no effect. In other words,
	//deleting a null pointer is a safe operation. There is no need to check the
	//pointer for null-ness before passing it to delete:
	for(unsigned int i=0; i<NNY_PROTOCOL_RANGE; i++)
		delete m_table[i];
}

void PacketDispatcher::dispatch(CMD packetCmd, Packet &capsule) {
	//Check if the packetCmd really exist
	//Must be between 0(excluded) and lower to NNY_PROTOCOL_RANGE(included)
	//Notation: ]0,NNY_PROTOCOL_RANGE]
	if (packetCmd <= NNY_PROTOCOL_RANGE && packetCmd > 0) {
		//Check if it have a functionoid assigned ('else' if NULL)
		if (m_table[packetCmd] != NULL) {
			m_table[packetCmd]->doit(capsule);
		} else {
			std::cout << "[PacketDispatcher::dispatch] Packet dropped; not set in table of interest!" << std::endl;
		}
	} else {
		std::cout << "[PacketDispatcher::dispatch] Packet dropped; out of protocol range!" << std::endl;
	}
}