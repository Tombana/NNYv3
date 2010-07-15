#include "CapsuleAuth.h"

void CapsuleAuth::doit(s_session* session, Packet& capsule) {
	std::cout << "[CapsuleAuth] Packet PCKT_C_AUTH received!" << std::endl;

	//Save input
	std::string username  = capsule.readString(); //input username
	std::string password  = capsule.readString(); //input password
	bool		kickGhost = capsule.readBool();   //input KickGhost flag 

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
	req << "SELECT w_accounts.id,LOWER(password),count(online) FROM w_accounts ";
	req << "LEFT OUTER JOIN w_characters ";
	req << "ON w_accounts.id = w_characters.accountID AND w_characters.online=1 ";
	req << "WHERE w_accounts.username=LOWER('" << cleanUsername << "')";

	//Prepare the response packet
	Packet packetToSend;
	packetToSend.add<CMD>(PCKT_W_AUTH_ACK);

	//Query the database
	database::result result = database::query(g_db, req, database::STORE_RESULT);

	//Check result is valid in case of mean hackers
	if (result) {
		database::row row = database::fetch_row(result);
		if (row) {
			std::cout << "-> row " << row[0] << " " << row[1] << " " << row[2] << std::endl;
			std::string db_password = row[1];
			if (db_password == password) {
				//TODO: check kickGhost and do what you're supposed to

				//If multiple login isn't allowed, check if we don't have a char logged already
				if (!CONFIG_ALLOW_MULTIPLE_LOGIN && database::toInt(row[2)) > 0) {
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
	}
}