#ifndef H_THREADHANDLER
#define H_THREADHANDLER

#include "ZSocket.h"
#include "pthread.h"
#include "ByteArray.h"
#include "Database.h"
#include "Dispatcher.h"
#include "config.hpp"

void *threadHandler(void *ptr);

//! s_thread_data is a structure that contains all datas a thread needs to work properly.
/** pthread_create allows only one parameter to be passed to the thread. \n
* We have tons of stuff the thread needs to know so we pass a structure instead. \n
* \sa We needs to copy all theses variables locally (in the thread) because what
we sent is no more than a pointer \b and so the \b real structure will be destroyed at the end of the main() loop.
*/
struct s_thread_data {
    SOCKET           socketID;
    Database        *database;
    Dispatcher      *dispatcher;
    bool             is_a_worldserver;
    unsigned int     worldserverID;
    pthread_mutex_t  memory_protect_mutex;
    pthread_cond_t   memory_protect_signal;
    bool             memory_flag_done_copying;
};

#endif
