#include "Dispatcher.h"

/* TODO (NitriX#): Well, it kind of check sockets... but the socket get destroyed... so we might use ZSocket
objects instead of a multiplexer. */

//################################################
//##            Constructor
//################################################
Dispatcher::Dispatcher(Database &database): m_database(database) {
    pthread_mutex_init(&m_v_x_worldservers, NULL);
    //m_v_x_worldservers = PTHREAD_MUTEX_INITIALIZER;
    _getDBWorldserverList();
    //_getDBDefaultWorldserver();
    m_currentWorldserverIndex = 0;
    m_acceptNextWorldserverAsCurrent = true;
}

//################################################
//##            Destructor
//################################################
Dispatcher::~Dispatcher() {
}

//################################################
//##            _getDBDefaultWorldserver
//################################################
/*
void Dispatcher::_getDBDefaultWorldserver() {
    std::cout << "[Dispatcher] Searching database for default worldserver... ";
    DB_RESULT result = m_database.query(DB_USE_RESULT, "SELECT worldserver_id FROM config");
    DB_ROW rows      = m_database.fetch_row(result);
    unsigned int search_default_worldserver = atoi(rows[0]);
    m_database.queryDone();
    std::cout << "OK!" << std::endl;

    unsigned int max = m_v_worldservers.size();
    for (unsigned int i=0; i<max; i++) {
        if (m_v_worldservers[i].id == search_default_worldserver) {
            std::cout << "[Dispatcher] Default worldserver is set on " << m_v_worldservers[i].name << std::endl;
            m_default_worldserver_index = i;
            break;
        }
    }
}
*/

//################################################
//##            printServers
//################################################
void Dispatcher::printServers() {
std::cout << "[Dispatcher] ----------------------- " << std::endl;
for (unsigned int i = 0; i<m_v_worldservers.size(); i++) {
    std::cout << "[Dispatcher] " << m_v_worldservers[i].id << " - " << m_v_worldservers[i].name << " - " <<
    m_v_worldservers[i].ipv4 << ":" << m_v_worldservers[i].port << " - " << m_v_worldservers[i].sync_key << " [";
    if (m_v_worldservers[i].online) {
        printColor("Online", COLOR_GREEN);
    } else {
        printColor("Offline", COLOR_RED);
    }
    std::cout << "]" << std::endl;
}
std::cout << "[Dispatcher] ----------------------- " << std::endl;
}

//################################################
//##            pickupCurrent
//################################################
Dispatcher::s_current_worldserver Dispatcher::pickupCurrent() {
    pthread_mutex_lock(&m_v_x_worldservers);
    s_current_worldserver current;
    if (m_v_worldservers[m_currentWorldserverIndex].online) {
        current.ipv4  = m_v_worldservers[m_currentWorldserverIndex].ipv4;
        current.port  = m_v_worldservers[m_currentWorldserverIndex].port;
        current.gone  = false;
    } else {
        current.gone = true;
    }
    pthread_mutex_unlock(&m_v_x_worldservers);
    return current;
}

//################################################
//##            setOffline
//################################################
void Dispatcher::setOffline(unsigned int serverID) {
    pthread_mutex_lock(&m_v_x_worldservers);
    unsigned int max = m_v_worldservers.size();
    for (unsigned int i = 0; i<max; i++) {
        if (m_v_worldservers[i].id == serverID) {
            std::cout << "[Dispatcher] Server '" << m_v_worldservers[i].name << "' logged off!" << std::endl;
            m_v_worldservers[i].online = false;
            if (m_currentWorldserverIndex == i) {
                std::cout << "[Dispatcher] We lost our MASTER server!" << std::endl;
                bool weFoundAnAlternative = false;
                for (unsigned int j = 0; j<max; j++) {
                    if (m_v_worldservers[j].online) {
                        std::cout << "[Dispatcher] Slave server '" << m_v_worldservers[j].name << "' has been promoted to MASTER!" << std::endl;
                        m_currentWorldserverIndex = j;
                        weFoundAnAlternative = true;
                        break;
                    }
                }
                if (!weFoundAnAlternative) m_acceptNextWorldserverAsCurrent = true;
            }
            break;
        }
    }
    pthread_mutex_unlock(&m_v_x_worldservers);
}

//################################################
//##            setOnline
//################################################
void Dispatcher::setOnline(unsigned int serverID) {
    pthread_mutex_lock(&m_v_x_worldservers);
    unsigned int max = m_v_worldservers.size();
    for (unsigned int i = 0; i<max; i++) {
        if (m_v_worldservers[i].id == serverID) {
            std::cout << "[Dispatcher] Server '" << m_v_worldservers[i].name << "' joined!" << std::endl;
            m_v_worldservers[i].online = true;
            if (m_acceptNextWorldserverAsCurrent) {
                std::cout << "[Dispatcher] " << m_v_worldservers[i].name << " is now the MASTER server!" << std::endl;
                m_currentWorldserverIndex = i;
                m_acceptNextWorldserverAsCurrent = false;
            } else {
                std::cout << "[Dispatcher] " << m_v_worldservers[i].name << " is now a SLAVE server!" << std::endl;
            }
            break;
        }
    }
    pthread_mutex_unlock(&m_v_x_worldservers);
}

//################################################
//##            getDBWorldserverList
//################################################
void Dispatcher::_getDBWorldserverList() {
    std::cout << "[Dispatcher] Retrieving database worldserver list... ";
    DB_RESULT result = m_database.query(DB_USE_RESULT, "SELECT id,name,ipv4,port,sync_key FROM worldservers");
    DB_ROW rows;
    while (rows = m_database.fetch_row(result)) {
        s_worldserver worldserverToAppend;
        worldserverToAppend.id = (unsigned)atoi(rows[0]);
        worldserverToAppend.name = rows[1];
        worldserverToAppend.ipv4 = rows[2];
        worldserverToAppend.port = (unsigned)atoi(rows[3]);
        worldserverToAppend.sync_key = rows[4];
        worldserverToAppend.online = false;
        m_v_worldservers.push_back(worldserverToAppend);
    }
    m_database.queryDone();
    std::cout << "OK!" << std::endl;
}

void printColor(std::string &str, BYTE color) {
    printColor(str.c_str(), color);
}

void printColor(const char *str, BYTE color) {
    #if defined(WIN32)
        HANDLE hConsole;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        std::cout << str;
        SetConsoleTextAttribute(hConsole, COLOR_DEFAULT);
    #else
        std::cout << "\033[" << color << "m" << str << "\033[" << COLOR_DEFAULT << "m";
    #endif
}
