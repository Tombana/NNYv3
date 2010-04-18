#include "main.h"

//--------------- GLOBAL SECTION -----------------
SOCKET          g_threadStartupData_socketID; //Protected with g_threadPool_mutex
//The threadpool over-abuse the same two vars all over: g_threadPool_mutex and g_threadPool_cond
//We no need arrays or anything else.
//Locks are cummulative, all threads can use thoses two same vars then :)
pthread_mutex_t g_threadPool_mutex            = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
pthread_cond_t  g_threadPool_cond             = PTHREAD_COND_INITIALIZER; //Protected with g_threadPool_mutex
pthread_mutex_t g_threadPool_counter_mutex    = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
int             g_threadPool_counter_thread   = 0; //Protected with g_threadPool_counter_mutex
int             g_threadPool_counter_job      = 0; //Protected with g_threadPool_counter_mutex

int main() {
    //=========================================
    //                TO-DOs
    //=========================================
    //TODO (NitriX#): read config from config.ini
    //TODO (NitriX#): Omg log system! :<
    //TODO (NitriX#): What with packets hash? CRC32 please...

    //=========================================
    //            STARTUP MESSAGE
    //=========================================
    printStartupMessage();

    //=========================================
    //          START LISTENING PORT
    //=========================================
    std::cerr << "[main] Listening on port " << CONFIG_SERVER_PORT << "... ";
    ZSocket mainsocket; //Creating main server socket
    if (mainsocket.socket_bind(CONFIG_SERVER_PORT)) {
        std::cerr << "OK!" << std::endl;
    } else {
        std::cerr << "Failed!" << std::endl << "@ERROR: Unable to bind socket on port 6131" << std::endl;
        exit(EXIT_FAILURE);
    }

    //=========================================
    //          CONTACT REALM SERVER
    //=========================================
    //TODO (NitriX#): Put the server online

    //=========================================
    //          CREATING THREADPOOL
    //=========================================
    createNbThreadWorker(CONFIG_THREADPOOL_DEFAULT_WORKER);
    std::cerr << "World server is ready!" << std::endl;

    //=========================================
    //         WAITING FOR CONNECTIONS
    //           [MAIN PROGRAM LOOP]
    //=========================================
    std::cerr << "Waiting for connections..." << std::endl;
    while (true) {
        //IMPORTANT THREAD STARTUP DATA HERE!
        //-----------------------------------------------
        g_threadStartupData_socketID = mainsocket.socket_acceptClient();
        //-----------------------------------------------

        std::cerr << "New client!" << std::endl;

        //ThreadPool :D
        pthread_mutex_lock(&g_threadPool_counter_mutex);
            g_threadPool_counter_job++;
            int counter_job = g_threadPool_counter_job;
            int counter_thread = g_threadPool_counter_thread;
            #if CONFIG_VERBOSE >= 2
                std::cerr << "[threadPoolHandler] " << g_threadPool_counter_job << " active jobs in " <<
                g_threadPool_counter_thread << " threads." << std::endl;
            #endif
        pthread_mutex_unlock(&g_threadPool_counter_mutex);

        //Check if all threads are used
        if (counter_job == counter_thread) {
            //Check if can still create more threads
            if (counter_thread < CONFIG_THREADPOOL_MAX_WORKER) {
                //Do not create too many threads! Make sure it isnt bigger than MAX_WORKER
                if (counter_thread+CONFIG_THREADPOOL_ADD_WORKER > CONFIG_THREADPOOL_MAX_WORKER) {
                    createNbThreadWorker(CONFIG_THREADPOOL_MAX_WORKER-counter_thread);
                } else {
                    createNbThreadWorker(CONFIG_THREADPOOL_ADD_WORKER);
                }
            } else {
                //TODO (NitriX#): We reached the thread limit!
                std::cerr << "We reached the maximum thread limit!" << std::endl;
                while (true) {
                    sleep(1);
                }
            }
        }

        pthread_cond_signal(&g_threadPool_cond); //Wake up a thread by sending this signal on the waiting condition

    }

    return 0;
}

void printStartupMessage() {
    std::cerr <<
    "/-----" << std::endl <<
    "| ~ NNYv3 World Server ~" << std::endl <<
    "| \t" << BUILD_VERSION << " [Build " << BUILD_NUMBER << "]" << std::endl <<
    "|" << std::endl <<
    "| Server details: \t\t\t\t" << std::endl <<
    "| \tBuild date: " << BUILD_DATE << std::endl <<
    "| \tDatabase supported: v" << CONFIG_DATABASE_SUPPORTED << std::endl <<
    "|" << std::endl <<
    "| Libraries/classes version: \t\t\t\t" << std::endl <<
    "| \tServer protocol: v" << PROTOCOL_VERSION << std::endl <<
    "| \tZSocket: " << ZSOCKET_VERSION << std::endl <<
    "|" << std::endl <<
    "| Recent modifications:" << std::endl <<
    "|   ThreadPool implemented, finally!" << std::endl <<
    "|   Forked & very edited from Realm sources" << std::endl <<
    "\\-----" << std::endl;
}

void createNbThreadWorker(int amount) {
    for (int i=0; i<amount; i++) { //we create as many worker we needs to
        //Create new thread
        pthread_t thread;
        //Counting threads...
        pthread_mutex_lock(&g_threadPool_counter_mutex);
        g_threadPool_counter_thread++;
        pthread_mutex_unlock(&g_threadPool_counter_mutex);

        //Launching thread and send the threadData structure memory address as parameters
        int rc=0;
        rc = pthread_create(&thread, NULL, threadPoolHandler, NULL); //here we launch the thread
        rc = pthread_detach(thread); //Detach thread so it works on its own
        //Checking errors
        if (rc) std::cerr << "[main] @ERROR: pthread: pthread_create() failed!" << std::endl;
    }
    #if CONFIG_VERBOSE >= 2
        std::cerr << "[main] " << amount << " thread worker has been created!" << std::endl;
    #endif
}
