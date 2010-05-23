#ifndef H_MAIN
#define H_MAIN

#include <fstream>
#include "ZSocket.h"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif
#include "config.hpp"
//#include "threadPoolHandler.h"
#include "sharedModels.h"
#include "Grid.h"
#include "Database.h"
#include <list>

#include <iostream>
#include "Thread.h"
#include "ThreadRealmConnector.h"

//TODO: simply testing, must be removed
#include "Acceptor.h"

int main();
//void createNbThreadWorker(int amount);
void checkDatabaseVersion();

#ifdef WIN32
    #include <windows.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
    #define sleep(x) usleep(x)
#endif
void pauseServer();

#endif
