#include "CapsuleWorldlist.h"

void CapsuleWorldlist::doit(SESSION session, Packet& capsule) {
	std::cout << "[CapsuleWorldlist] PCKT_C_GETWORLDLIST received!" << std::endl;
	
	Packet packetToSend;

	//Send SQL query, the oldest server will be sent first
	database::result db_result = database::query(g_db, "SELECT id,ipv4,port,name FROM servers ORDER BY id ASC", database::STORE_RESULT);
	//Dealing with results
	if (db_result) {
		database::row row;
		while (row = database::fetch_row(db_result)) {
			unsigned int id = database::toInt(row[0]); //convert and save id
			
			packetToSend.add<CMD>(PCKT_R_WORLD);
			packetToSend.addString(row[1]); //ipv4
			packetToSend.add<DWORD>(database::toInt(row[2])); //port
			packetToSend.addString(row[3]); //name

			//TODO: ONLINE
			packetToSend.addBool(true); //online, static for now hence the system isn't implemented yet
		}
		database::free_result(db_result);
	}
	packetToSend.add<CMD>(PCKT_R_WORLD_EOF);
	session->socket.write(packetToSend);
}