#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <vector>
#include "config.hpp"
#include "ZSocket.h"
#include "pthread.h"
#include "version.hpp"
#include "threadHandler.h"
#include "Dispatcher.h"
#include "Database.h"
#include <fstream>

int main();
void checkDatabase(Database &database);

#endif
