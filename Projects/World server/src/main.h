#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include "ZSocket.h"
#include "pthread.h"
#include "config.hpp"
#include "version.hpp"
#include "threadPoolHandler.h"
#include "realmConnector.h"

int main();
void printStartupMessage();
void createNbThreadWorker(int amount);

#endif
