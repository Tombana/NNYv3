#ifndef H_DATABASE
#define H_DATABASE

#ifdef WIN32
        #include "winsock2.h" //Needed by MySQL lib for sockets
        #include "mysql.h" //MySQL library
        #include "errmsg.h" //MySQL handling error messages
#else
        #include <mysql/mysql.h> //linux version: libmysqlclient
        #include <mysql/errmsg.h> //linux version: libmysqlclient
#endif

#include <stdlib.h> //atoi() include

#include <iostream>
#include <string> //sql type
#include <sstream>

namespace database {
	typedef MYSQL* connection;
	typedef MYSQL_RES* result;
	typedef MYSQL_ROW row;
	typedef std::stringstream sql;
	enum model_result { STORE_RESULT, USE_RESULT }; 

	///Send a query to the MySQL server and expect a result
	result query(connection conn, const char *request, model_result type);

	///Send a query to the MySQL server but don't expect a result
	void execute(connection conn, const char *request);

	///Handle errors, shouldn't be used except in very rare situations
	void handleError(connection coon, int errorCode);

	//============= INLINE ==============
	///Send a query to the MySQL server and expect a result
	inline result query(connection conn, sql& request, model_result type) {
		return query(conn, request.str().c_str(), type);
	}
	
	///Send a query to the MySQL server but don't expect a result
	inline void execute(connection conn, sql& request) {
		execute(conn, request.str().c_str());
	}

	///Load mysql library
	inline void load() {
		mysql_library_init(0, NULL, NULL);
	}

	///Unload mysql libraru
	inline void unload() {
		mysql_library_end();
	}

	///Close a connection to the server
	inline void close(connection conn) {
		mysql_close(conn);
	}

	///Connect to the MySQL server
	inline bool connect(connection conn, const char *host, const char *username, const char *password, const char *database, unsigned int port) {
		return mysql_real_connect(conn, host, username, password, database, port, NULL, 0) ? true : false;
	}

	///Prepare a connection to contact the MySQL server
	inline connection init() {
		return mysql_init(NULL);
	}

	///Fetch a row of results
	inline row fetch_row(result res) {
		//Here there's two possible output, a row or NULL.
		//The only way to handle the `NULL` state would be to call mysql_errno(MYSQL*)
		//but we don't have the MYSQL* so I'll simply assume we are at the end of the result set
		//and that no error occured. Actually, the two possible errors are: (with USE_RESULT flag)
		//	CR_SERVER_LOST   - The connection to the server was lost during the query.
		//	CR_UNKNOWN_ERROR - An unknown error occurred.
		//The second one doesn't scares me much, but CR_SERVER_LOST can be annoying... that's
		//why I -VIVIDLY- recommend you uses STORE_RESULT flag, wich call mysql_store_result
		//and then doesn't need a connection anymore because it's saved to memory.
		return mysql_fetch_row(res); //TODO: Error handling if the flag was USE_RESULT earlier
	}

	///Free result
	inline void free_result(result res) {
		mysql_free_result(res);
	}

	///Conversion to int
	inline int toInt(const char* str) {
		return atoi(str);
	}

	///We want to know the sql error
	inline const char *error(connection conn) {
		return mysql_error(conn);
	}
}

#endif
