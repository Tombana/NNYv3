#ifndef H_CAPSULE_VERSION
#define H_CAPSULE_VERSION

//Relative path here; we are in a folder nammed "Casules/".
#include "../Capsule.h"
//Virtual path here; these are included by the project settings
#include "database.h"
#include "Packet.h"
#include "resProtocol.h"

extern unsigned int g_client_version;
extern database::connection g_db;

class CapsuleVersion : public Capsule {
public:
	virtual void doit(SESSION session, Packet& capsule) {
		std::cout << "[CapsuleVersion] Packet received!" << std::endl;
		DWORD version = capsule.read<DWORD>();
		std::cout << "Version: " << version << std::endl;

		Packet packetToSend;

		//Check the client version supported
		if (version > g_client_version) {
			//I don't mind hackers; just don't play with me.
			packetToSend.add<CMD>(PCKT_X_HACKING_ATTEMPT);
			session->socket.write(packetToSend);
		} else {
			if (version != g_client_version)  {
				//Prepare SQL query, all updates > version but < g_client_version
				database::sql sql;
				sql << "SELECT version,url,path FROM updates WHERE version>" << version << " AND version<" << g_client_version << " ORDER BY version ASC";
				//Send SQL query
				database::result db_result = database::query(g_db, sql, database::STORE_RESULT);
				//Dealing with results
				if (db_result) {
					database::row row;
					while (row = database::fetch_row(db_result)) {
						packetToSend.add<CMD>(PCKT_R_DOWNLOAD);
						packetToSend.add<DWORD>(database::toInt(row[0])); //(int) version
						packetToSend.addString(row[1]); //url
						packetToSend.addString(row[2]); //path
					}
					database::free_result(db_result);
				}
			}
			packetToSend.add<CMD>(PCKT_R_DOWNLOAD_EOF);
			session->socket.write(packetToSend);
		}
	} //end of doit()
}; //end of class

#endif
