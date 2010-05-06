#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <fstream>
#include "ZSocket.h"
#if defined(WIN32)
     #include "pthread.h"
#else
     #include <pthread.h>
#endif
#include "config.hpp"
#include "threadPoolHandler.h"
#include "realmConnector.h"
#include "sharedModels.hpp"
#include "Grid.h"
#include "Database.h"

int main();
void printStartupMessage();
void createNbThreadWorker(int amount);
void genVersion();

#endif
