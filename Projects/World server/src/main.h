#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <fstream>
#include "ZSocket.h"
#include "pthread.h"
#include "config.hpp"
#include "threadPoolHandler.h"
#include "realmConnector.h"
#include "sharedModels.hpp"
#include "grid.h"

int main();
void printStartupMessage();
void createNbThreadWorker(int amount);
void genVersion();

#endif
