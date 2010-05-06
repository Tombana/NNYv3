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

//=============================
//       THREAD DATA
//=============================
//s_thread_data is a structure that contains all datas
//a thread needs to work properly.
struct s_thread_data {
    ZSocket socket;
};

//grid.h is placed here because it needs s_thread_data to be declared when included
#include "Grid.h"

void threadHandler(SOCKET &m_socketID);

#endif
