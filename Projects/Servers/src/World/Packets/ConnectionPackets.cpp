#include "ConnectionPackets.h"

//==============================
// PCKT_C_AUTH
//==============================
void ConnectionPackets::auth(SESSION& session, Packet& input) {
	//Display a debugging message
	std::cout << "[ConnectionPackets][auth] PCKT_C_AUTH received!" << std::endl;

	//Save input
	std::string username  = input.readString(); //input username
	std::string password  = input.readString(); //input password
	bool		kickGhost = input.readBool();   //input KickGhost flag 

	//Clean username
	std::string cleanUsername;

	//Sanitize just in case
	for (std::string::iterator itr = username.begin(); itr < username.end(); itr++) {
		if (((*itr)<='z' && (*itr)>='a') || ((*itr)<='Z' && (*itr)>='A')) {
			cleanUsername += (*itr);
		}
	}

	//Transform password to lower case for comparaison
	std::transform(password.begin(), password.end(), password.begin(), tolower);

	//TODO: more stuff here
	std::cout << "cleanUsername is: " << cleanUsername << std::endl;
	std::cout << "password is: " << password << std::endl;
	std::cout << "kickGhost flag is: " << kickGhost << std::endl;

	//Prepare the query
	database::sql req;
	req << "SELECT count(w_accounts.id),w_accounts.id,LOWER(password),count(online) FROM w_accounts ";
	req << "LEFT OUTER JOIN w_characters ";
	req << "ON w_accounts.id = w_characters.accountID AND w_characters.online=1 ";
	req << "WHERE w_accounts.username=LOWER('" << cleanUsername << "')";

	//Prepare the response packet
	Packet packetToSend;
	packetToSend.add<CMD>(PCKT_W_AUTH_ACK);

	//Query the database
	database::result result = database::query(g_db, req, database::STORE_RESULT);

	//Check if result is valid just in case it would impossibly fail.
	if (result) {
		database::row row = database::fetch_row(result);
		//BE AWARE! Because this use JOIN & COUNT() at the same time, we will ALWAYS get 1 result, not 0, not 2.
		//Therefore we must know how many entries were really found with another count().
		if (database::toInt(row[0])) { ///<-- this is the other count()
			std::string db_password = row[2];
			if (db_password == password) {
				//TODO: check kickGhost and do what you're supposed to

				//If multiple login isn't allowed, check if we don't have a char logged already
				if (!CONFIG_ALLOW_MULTIPLE_LOGIN && database::toInt(row[3]) > 0) {
					packetToSend.add<ACK>(ACK_ALREADY);
				} else {
					packetToSend.add<ACK>(ACK_SUCCESS);
					//TODO: MORE IN-GAME STUFF HERE
				}
			} else {
				packetToSend.add<ACK>(ACK_DOESNT_MATCH);
			}
		} else {
			packetToSend.add<ACK>(ACK_NOT_FOUND);
		}
		database::free_result(result);
	} else {
		packetToSend.add<ACK>(ACK_INTERNAL_ERROR);
	}

	session->socket.write(packetToSend);
}