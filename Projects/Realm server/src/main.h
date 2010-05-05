#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <vector>
#include "config.hpp"
#include "ZSocket.h"
#if defined(WIN32)
    #include "pthread.h"
#else
    #include <pthread.h>
#endif
#include "threadHandler.h"
#include "Dispatcher.h"
#include "Database.h"
#include <fstream>

int main();
void checkDatabase(Database &database);

#endif
