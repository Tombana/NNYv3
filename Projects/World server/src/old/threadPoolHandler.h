#ifndef H_THREADPOOLHANDLER
#define H_THREADPOOLHANDLER

#include "ZSocket.h"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif
#include "ByteArray.h"
#include "config.hpp"
#include "threadHandler.h"

void *threadPoolHandler(void *ptr);

#endif
