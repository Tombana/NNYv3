#ifndef H_DISPATCHER
#define H_DISPATCHER

#include <iostream>
#include <vector>
#include "pthread.h"
#include "config.hpp"
#include "ZSocket.h"
#include "mysql.h"
#include "Database.h"

class Dispatcher {
public:
    typedef struct {
        std::string ipv4;
        WORD port;
        bool gone;
    } s_current_worldserver;
private:
    typedef struct {
        DWORD id;
        std::string name;
        std::string ipv4;
        WORD port;
        std::string sync_key;
        bool online;
    } s_worldserver;

public:
    Dispatcher::Dispatcher(Database &database);
    ~Dispatcher();
    //void pickNextAvailableServer(bool tryToConnectEvenIfNotOnline=false);
    s_current_worldserver pickupCurrent();
    void printServers();
    void setOnline(unsigned int serverID);
    void setOffline(unsigned int serverID);

private:
    void            _connectToMySQL();
    void            _getDBWorldserverList();
    void            _getDBDefaultWorldserver();
    void            _connectToWorldservers();

    //Database
    Database                    &m_database;
    //Ressources
    std::vector<s_worldserver>   m_v_worldservers;
    unsigned int                 m_currentWorldserverIndex;
    bool                         m_acceptNextWorldserverAsCurrent;
    //Mutexes
    pthread_mutex_t              m_v_x_worldservers;
};

//Console color
#if defined(WIN32)
    #define COLOR_GREEN     10
    #define COLOR_RED       12
    #define COLOR_DEFAULT   7
#endif

void printColor(std::string &str, BYTE color);
void printColor(const char *str, BYTE color);

#endif
