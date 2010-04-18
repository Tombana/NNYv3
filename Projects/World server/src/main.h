#ifndef H_MAIN
#define H_MAIN

#include <iostream>
#include <vector>
#include "ZSocket.h"
#include "pthread.h"
#include "config.hpp"
#include "version.hpp"
#include "threadPoolHandler.h"

int main();
void printStartupMessage();
void createNbThreadWorker(int amount);

#endif
