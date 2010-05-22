#include "main.h"

//Loading configuration file CONFIG_FILENAME defined in config.hpp
ConfigFile g_CONFIG(CONFIG_FILENAME);

// == THREADPOOL ==
// The threadpool over-abuse the same two vars all over: g_threadPool_mutex and g_threadPool_cond
// We no need arrays or anything else.
// Locks are cummulative, all threads can use thoses two same vars then :)
SOCKET          g_threadStartupData_socketID; //Protected with g_threadPool_mutex
pthread_mutex_t g_threadPool_callback_mutex   = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
pthread_cond_t  g_threadPool_callback_cond    = PTHREAD_COND_INITIALIZER; //Protected with g_threadPool_callback_mutex
pthread_mutex_t g_threadPool_mutex            = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
pthread_cond_t  g_threadPool_cond             = PTHREAD_COND_INITIALIZER; //Protected with g_threadPool_mutex
pthread_mutex_t g_threadPool_counter_mutex    = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
unsigned int    g_threadPool_counter_thread   = 0; //Protected with g_threadPool_counter_mutex
unsigned int    g_threadPool_counter_job      = 0; //Protected with g_threadPool_counter_mutex
// == Grid class requirement ==
Grid                      g_grid; //Grid object is thread-safe
// == Database class requirement ==
Database                  g_database; //Database object is thread-safe
// == Online (authenticated/logged) user list ==
pthread_mutex_t g_onlineList_mutex            = PTHREAD_MUTEX_INITIALIZER; //MUTEX! :)
std::list<s_thread_data*> g_onlineList; //Protected with g_onlineList_mutex
//-------------------------------------------------

int main() {
    //=========================================
    //               ALL TODOs
    //=========================================
    /// \todo We need a log system!

    //=========================================
    //            STARTUP MESSAGE
    //=========================================
    std::cout << "NN      NN  NN      NN NN    NN             "   << std::endl;
    std::cout << "NNNN    NN  NNNN    NN  NN  NN        NNNNN "   << std::endl;
    std::cout << "NN NN   NN  NN NN   NN   NNNN            NN "   << std::endl;
    std::cout << "NN  NN  NN  NN  NN  NN    NN  NN    NN  NN  "   << std::endl;
    std::cout << "NN   NN NN  NN   NN NN    NN  NN   NN  NNN  "   << std::endl;
    std::cout << "NN    NNNN  NN    NNNN    NN   NN NN     NN "   << std::endl;
    std::cout << "NN     NNN  NN     NNN    NN    NNN   NNNN  "   << std::endl;
    std::cout << "Using configuration file " << CONFIG_FILENAME   << std::endl;
    std::cout << "Using server protocol " << NNY_PROTOCOL_VERSION << std::endl << std::endl;

    //=========================================
    //            MYSQL DATABASE
    //=========================================
    std::cerr << "Connecting to MySQL database... ";
    if (g_database.connect(CONFIG_MYSQL_SERVER, CONFIG_MYSQL_USERNAME, CONFIG_MYSQL_PASSWORD, CONFIG_MYSQL_DATABASE)) {
        std::cerr << "OK!" << std::endl;
        checkDatabaseVersion();
    } else {
        std::cerr << "Failed!" << std::endl;
        std::cerr << "@ERROR: Unable to connect to the MySQL database! Please check world.conf file!" << std::endl;
        pauseServer();
    }

    //=========================================
    //               MAP GRID
    // Create the Grid and the default map 0
    //=========================================
    std::cerr << "Creating map grid... ";
    g_grid.createMap(0); //default map
    std::cerr << "OK!" << std::endl;

    //=========================================
    //          CREATING THREADPOOL
    //=========================================
    createNbThreadWorker(CONFIG_THREADPOOL_DEFAULT_WORKER);
    //TODO: wait for some threads to be created?!
    //there should be a check here
    std::cerr << "World server is ready!" << std::endl;

    //=========================================
    //          START LISTENING PORT
    //=========================================
    std::cerr << "Listening on port " << CONFIG_SERVER_PORT << "... ";
    ZSocket mainsocket; //Creating main server socket
    if (mainsocket.socket_bind(CONFIG_SERVER_PORT)) {
        std::cerr << "OK!" << std::endl;
    } else {
        std::cerr << "Failed!" << std::endl << "@ERROR: Unable to bind socket on port " << CONFIG_SERVER_PORT << std::endl;
        exit(EXIT_FAILURE);
    }

    //=========================================
    //            REALM CONNECTOR
    //  Create a threaded class and start it
    //=========================================
    //create the realm connector object
    ThreadRealmConnector threadRealmConnector;
    //start the thread
    threadRealmConnector.start();

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

        //ThreadPool: counters
        pthread_mutex_lock(&g_threadPool_counter_mutex);
            g_threadPool_counter_job++;
            unsigned int counter_job = g_threadPool_counter_job;
            unsigned int counter_thread = g_threadPool_counter_thread;
            #if CONFIG_VERBOSE >= CONFIG_VERBOSE_DEBUGGING
                std::cerr << "[threadPoolHandler] " << g_threadPool_counter_job << " active jobs in " <<
                g_threadPool_counter_thread << " threads." << std::endl;
            #endif
        pthread_mutex_unlock(&g_threadPool_counter_mutex);

        //ThreadPool: Check threads usage and perform action accordingly
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
                pauseServer();
            }
        }

        //TODO: what if threads are created but none is running here? To investigate
        pthread_cond_signal(&g_threadPool_cond); //Wake up a thread by sending this signal on the waiting condition
        pthread_cond_wait(&g_threadPool_callback_cond, &g_threadPool_callback_mutex); //Wait for the thread to copy vars locally
        //pthread_mutex_lock(&g_threadPool_mutex); //Try to acquire the lock (so wait until the thread really is done
        //pthread_mutex_unlock(&g_threadPool_mutex); //Release the lock
    }

    //===================================
    // Exit
    //===================================
    return 0;
}

void createNbThreadWorker(int amount) {
    for (int i=0; i<amount; i++) { //we create as many worker we needs to
        //Create new thread
        pthread_t thread;
        //Counting threads...
        pthread_mutex_lock(&g_threadPool_counter_mutex);
        g_threadPool_counter_thread++; //Tread-safety: here is a mutex because threads may decrement(--) this variable
        pthread_mutex_unlock(&g_threadPool_counter_mutex);

        //Launching thread and send the threadData structure memory address as parameters
        int rc=0;
        rc = pthread_create(&thread, NULL, threadPoolHandler, NULL); //here we launch the thread
        rc |= pthread_detach(thread); //Detach thread so it works on its own
        //Checking errors
        if (rc) std::cerr << "[main] @ERROR: pthread: pthread_create() failed!" << std::endl;
    }
    #if CONFIG_VERBOSE >= CONFIG_VERBOSE_DEBUGGING
        std::cerr << "[main] " << amount << " thread worker has been created!" << std::endl;
    #endif
}

void pauseServer() {
    while(true) {
        sleep(5000); //5 seconds pause
    }
}

void checkDatabaseVersion() {
    //----- Execute the query
    DB_RESULT *result = g_database.query(DB_USE_RESULT, "SELECT db_version FROM config");
    DB_ROW     rows   = g_database.fetch_row(result);
    //----- Do stuff we wants with rows[]
    unsigned int db_version = atoi(rows[0]);
    //----- Free ressources as soon as possible
    g_database.queryDone();
    //----- Process the new data :)
    std::cout << "[main] Checking database version... ";
    if (db_version == CONFIG_DATABASE_SUPPORTED) {
        std::cout << "OK!" << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
        std::cout << "@ERROR: Your current database version is not supported!" << std::endl;
        std::cout << "@ERROR: Please install a clean 'fulldb' or run 'updates' from SQL folder." << std::endl;
        exit(EXIT_FAILURE);
    }
}
