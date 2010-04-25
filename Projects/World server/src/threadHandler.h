#ifndef H_THREADHANDLER
#define H_THREADHANDLER

#include "ZSocket.h"
#include "pthread.h"
#include "ByteArray.h"
#include "config.hpp"
#include "global_models.hpp"

void threadHandler(SOCKET &m_socketID);

//s_thread_data is a structure that contains all datas a thread needs to work properly.
struct s_thread_data {
    ZSocket socket;
};

#endif
