#include "Database.h"

Database::Database() {
    m_sql_result = NULL;
    pthread_mutex_init(&m_mutex, NULL);
    //m_mutex = PTHREAD_MUTEX_INITIALIZER;
    my_bool reconnect = true;
	m_mysql = mysql_init(NULL);
    mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &reconnect);
}

Database::~Database() {
}

bool Database::connect(const char *server, const char *username, const char *password, const char *database) {
    return mysql_real_connect(m_mysql,server,username,password,database,0,NULL,0);
}

void Database::close() {
    mysql_close(m_mysql);
}

MYSQL_RES* Database::query(bool store_result, const char *toSend) {
    //TODO: Save the query if it cannot be performed

    //----- Init
    int sql_error = 0;
    //----- Lock mutex
    pthread_mutex_lock(&m_mutex);
    //----- Execute query
    mysql_ping(m_mysql);
    sql_error = mysql_query(m_mysql, toSend);
    if (sql_error != 0) {
        std::cout << "@ERROR: MySQL: " << mysql_error(m_mysql) << std::endl;
    }
    //----- Use/store result but DONT fetch it yet
    if (store_result) {
        m_sql_result = mysql_store_result(m_mysql);
    } else {
        m_sql_result = mysql_use_result(m_mysql);
    }
    //----- Return
    return m_sql_result;
}

MYSQL_RES* Database::query(bool store_result, std::string &obj) {
    return query(store_result, obj.c_str());
}

void Database::queryDone() {
    //----- Free ressources as soon as possible
    mysql_free_result(m_sql_result);
    //----- Unlock mutex
    pthread_mutex_unlock(&m_mutex);
}

MYSQL_ROW Database::fetch_row(MYSQL_RES *result) {
    return mysql_fetch_row(result);
}

std::string Database::intToStr(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

std::string alphaNumOnly(std::string strToConvert) {
	std::string strConverted; //strToConvert with only alpha-numeric characters.
	for (unsigned int i=0; i<strToConvert.length();i++) {
        if (isalnum(strToConvert[i])) //If current character is alpha-numeric
            strConverted += strToConvert[i]; //Add the character to strConverted
    }
    return strConverted;
}
