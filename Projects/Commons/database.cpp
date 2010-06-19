#include "database.h"

namespace database {
	//========================================
	// SEND A QUERY BUT DON'T EXPECT A RESULT
	//========================================
	void execute(connection conn, const char *request) {
		int error = mysql_query(conn, request);
		if (error) { //Zero = goodboy, everything else = badboy
			handleError(conn, error); //TODO: You may want to handle specific errors here to re-try later (i.e. connection dropped)
		}
	}

	//========================================
	//	  SEND A QUERY AND EXPECT A RESULT
	//========================================
	result query(connection conn, const char *request, model_result type) {
		int error = mysql_query(conn, request); //Zero = goodboy, everything else = badboy
		if (error) {
			handleError(conn, error);
			return NULL; //We had an error; return an empty `result` set.
		} else {
			if (mysql_field_count(conn)) { //Zero = badboy, everything else = mysql result
				//Create the output result set
				result output;
				//Store or use the result according to user needs
				if (type == USE_RESULT) {
					output = mysql_use_result(conn); //returns NULL when an error occurs
							//Then uses:
								//mysql_fetch_row() in a loop until NULL.
								//mysql_free_result()
				} else {
					output = mysql_store_result(conn); //returns NULL when an error occurs
							//Then uses:
								//mysql_fetch_row() and others functions... again until NULL.
								//mysql_free_result()
				}
				//checking output for `NULL`
				if (output) {
					return output;
				} else {
					handleError(conn, mysql_errno(conn));
					//***return NULL below***
				}
			} //Else, we expected data but we received nothing, ***return NULL below***
			return NULL; //Here we return an empty `result` set for all the references above
		}
	}

	//========================================
	//	       HUMAN-READABLE ERRORS
	//========================================
	void handleError(connection conn, int errorCode) {
		switch (errorCode) {
			case 0:
				std::cout << "@datase: Wtf, an error was detected but the errorCode is good" << std::endl;
				break;
			case CR_COMMANDS_OUT_OF_SYNC:
				std::cout << "@database: Commands out of sync" << std::endl;
				break;
			case CR_OUT_OF_MEMORY:
				std::cout << "@database: Out of memory" << std::endl;
				break;
			case CR_SERVER_GONE_ERROR:
				std::cout << "@database: Server is gone" << std::endl;
				break;
			case CR_SERVER_LOST:
				std::cout << "@database: Server connection lost during query" << std::endl;
				break;
			case CR_UNKNOWN_ERROR:
				std::cout << "@database: Unknown error" << std::endl;
				break;
			default:
				std::cout << "@database: Error: " << mysql_error(conn) << std::endl;
		}
	}
} //End of namespace `database`