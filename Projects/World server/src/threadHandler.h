#ifndef H_THREADHANDLER
#define H_THREADHANDLER

#include "ZSocket.h"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif
#include "ByteArray.h"
#include "config.hpp"
#include "sharedModels.hpp"
#include "Database.h"
#include <list>

//=============================
//        THREAD DATA
//=============================
//s_thread_data is a structure that contains all datas our thread needs to work properly.
struct s_thread_data {
    //Socket
    ZSocket socket; //is thread-safe
    //Thread infos (mostly used in the ghost kicking system)
    pthread_t       thread;
    //Authentification
    unsigned int    accountID;
    bool            authenticated;
    //Entering world
    bool            logged;
    //Character infos
    unsigned int    id;
    std::string     name;
    BYTE            level;
    bool            gender;
    unsigned int    x;
    unsigned int    y;
    unsigned int    z;
    unsigned int    map;
};

//grid.h is placed here because it needs s_thread_data to be declared when included
#include "Grid.h"

void threadHandler(SOCKET &m_socketID);

#endif
