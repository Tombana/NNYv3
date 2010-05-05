#ifndef H_REALMCONNECTOR
#define H_REALMCONNECTOR

#include <iostream>
#include "ZSocket.h"
#include "config.hpp"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif

void *realmConnector(void *ptr);

#endif
