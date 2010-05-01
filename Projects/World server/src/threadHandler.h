#ifndef H_THREADHANDLER
#define H_THREADHANDLER

#include "ZSocket.h"
#include "pthread.h"
#include "ByteArray.h"
#include "config.hpp"
#include "sharedModels.hpp"

//=============================
//       THREAD DATA
//=============================
//s_thread_data is a structure that contains all datas
//a thread needs to work properly.
struct s_thread_data {
    ZSocket socket;
};

//grid.h is placed here because it needs s_thread_data to be declared when included
#include "grid.h"

void threadHandler(SOCKET &m_socketID);

#endif
