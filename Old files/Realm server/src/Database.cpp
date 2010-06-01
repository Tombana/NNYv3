#include "Database.h"

Database::Database() {
    m_sql_result = NULL;
    pthread_mutex_init(&m_mutex, NULL);
    //m_mutex = PTHREAD_MUTEX_INITIALIZER;
    my_bool reconnect = 0;
	m_mysql = mysql_init(NULL);
    mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &reconnect);
}

Database::~Database() {
}

bool Database::connect() {
    std::cout << "[Database] Connecting to MySQL server... ";
    if (mysql_real_connect(m_mysql,CONFIG_MYSQL_SERVER,CONFIG_MYSQL_USERNAME,CONFIG_MYSQL_PASSWORD,CONFIG_MYSQL_DATABASE,0,NULL,0)) {
        std::cout << "OK!" << std::endl;
        return true;
    } else {
        std::cout << "Failed!" << std::endl << "@ERROR: MySQL: " << mysql_error(m_mysql) << std::endl;
        return false;
    }
}

void Database::close() {
    mysql_close(m_mysql);
    std::cout << "[Database] Connection closed!" << std::endl;
}

DB_RESULT Database::query(bool store_result, const char *toSend) {
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

DB_RESULT Database::query(bool store_result, std::string &obj) {
    return query(store_result, obj.c_str());
}

void Database::queryDone() {
    //----- Free ressources as soon as possible
    mysql_free_result(m_sql_result);
    //----- Unlock mutex
    pthread_mutex_unlock(&m_mutex);
}

DB_ROW Database::fetch_row(DB_RESULT result) {
    return mysql_fetch_row(result);
}

std::string Database::intToStr(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
