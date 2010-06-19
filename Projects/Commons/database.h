#ifndef H_DATABASE
#define H_DATABASE

#if defined(WIN32)
	#include "windows.h"
	#include "mysql.h" //MySQL library
	#include "errmsg.h" //MySQL handling error messages
#endif

#include <iostream>

namespace database {
	typedef MYSQL* connection;
	typedef MYSQL_RES* result;
	enum model_result { STORE_RESULT, USE_RESULT }; 

	///Prepare a connection to contact the MySQL server
	connection init();
	
	///Connect to the MySQL server
	bool connect(connection conn, const char *host, const char *username, const char *password, const char *database, unsigned int port);

	///Send a query to the MySQL server and expect a result
	result query(connection conn, const char *request, model_result type);

	void handleError(connection coon, int errorCode);

	///We want to know the sql error
	const char *error(connection conn);
}

#endif