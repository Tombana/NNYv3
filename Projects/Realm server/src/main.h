#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <vector>
#include "ZSocket.h"
#include "pthread.h"
#include "config.hpp"
#include "version.hpp"
#include "threadHandler.h"
#include "Dispatcher.h"
#include "Database.h"
//Useless at the moment
//#include "CCRC32.h"
//#include "httpHandler.h"
//#include "signal.h"
#include <fstream>

int main();
void checkDatabase(Database &database);
//void leaving(int sig);

//s_thread_data is defined in threadHandler.h

#endif
