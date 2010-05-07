#ifndef H_DATABASE
#define H_DATABASE

#include <iostream>
#include <sstream>
#include "ZSocket.h"
#include "mysql.h"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif

#define DB_USE_RESULT   false
#define DB_STORE_RESULT true

#define DB_RESULT       MYSQL_RES
#define DB_ROW          MYSQL_ROW

#include <cstdlib> //atoi();

class Database
{
	public:
		Database();
		~Database();
		bool             connect(const char *server, const char *username, const char *password, const char *database);
		void             query(std::string &obj);
		void             query(const char *toSend);
		MYSQL_RES*       query(bool store_result, std::string &obj);
		MYSQL_RES*       query(bool store_result, const char *toSend);
		MYSQL_ROW        fetch_row(MYSQL_RES *result);
		void             queryDone();
		void             close();

	private:
        MYSQL_RES       *m_sql_result;
        MYSQL           *m_mysql;
        pthread_mutex_t  m_mutex;
};

std::string alphaNumOnly(std::string strToConvert);
std::string intToStr(int number);

#endif
