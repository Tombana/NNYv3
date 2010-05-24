#include "main.h"

// As long as keepWorking is true, the spawned thread will keep printing to the screen.
static bool keepWorking = true;

// This is the function that will be running in the spawned thread.
void* threadFunc(void *arg)
{
	ACE_OS::sleep(ACE_Time_Value(0, 5000)); // Add an initial delay of half a second
	while(keepWorking)
	{
		std::cout << "Thread - yawn. That was a nice nap. I think I'll sleep a bit more." << std::endl;
		ACE_OS::sleep(1);
	}

	std::cout << "Thread - I'm done working for you!" << std::endl;
	return NULL;
}

int main(int argc, char **argv) {
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
    //std::cout << "Using configuration file " << CONFIG_FILENAME   << std::endl;
    std::cout << "Using server protocol " << NNY_PROTOCOL_VERSION << std::endl << std::endl;

	//=========================================
    //              ACE TESTING
    //=========================================
	ACE_Thread_Manager threadManager;
	ACE_thread_t threadId;
	
	// Create one thread running the thread function
	if (threadManager.spawn((ACE_THR_FUNC)threadFunc, NULL, 0, &threadId) == -1)
	{
		std::cerr << "Error spawning thread" << std::endl;
		return -1;
	}

	std::cout << "Main - thread spawned with thread id " << threadId << "." << std::endl;

	// Sleep for 5 seconds and let the thread do some work.
	ACE_OS::sleep(5);

	// Now stop the thread and wait for it to exit.
	keepWorking = false;
	std::cout << "Main - end of work shift. Waiting for all threads to exit." << std::endl;
	threadManager.wait();

	std::cout << "Main - done." << std::endl;

    //=========================================
    //               ALL TODOs
    //=========================================
    /// \todo We need a log system!

    //=========================================
    //            MYSQL DATABASE
    //=========================================
    /*
	std::cerr << "Connecting to MySQL database... ";
    if (g_database.connect(CONFIG_MYSQL_SERVER, CONFIG_MYSQL_USERNAME, CONFIG_MYSQL_PASSWORD, CONFIG_MYSQL_DATABASE)) {
        std::cerr << "OK!" << std::endl;
        checkDatabaseVersion();
    } else {
        std::cerr << "Failed!" << std::endl;
        std::cerr << "@ERROR: Unable to connect to the MySQL database! Please check world.conf file!" << std::endl;
        //pauseServer();
    }
	*/

    //=========================================
    //               MAP GRID
    // Create the Grid and the default map 0
    //=========================================
    /*
	std::cerr << "Creating map grid... ";
    g_grid.createMap(0); //default map
    std::cerr << "OK!" << std::endl;
	*/

    //=========================================
    //                ACCEPTOR
    // Note: NULL means the server will listen
    //       on IP address 0.0.0.0
    //=========================================
	//Acceptor acceptor(NULL, CONFIG_SERVER_PORT);
	
    //=========================================
    //            REALM CONNECTOR
    //  Create a threaded class and start it
    //=========================================
    //Create the realm connector object
    //ThreadRealmConnector threadRealmConnector;
    //Start the thread
    //threadRealmConnector.start();

    //=========================================
    //         WAITING FOR CONNECTIONS
    //           [MAIN PROGRAM LOOP]
    //=========================================
    std::cerr << "World server is ready!" << std::endl;
    /*
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
            }
        }

        //TODO: what if threads are created but none is running here? To investigate
        pthread_cond_signal(&g_threadPool_cond); //Wake up a thread by sending this signal on the waiting condition
        pthread_cond_wait(&g_threadPool_callback_cond, &g_threadPool_callback_mutex); //Wait for the thread to copy vars locally
        //pthread_mutex_lock(&g_threadPool_mutex); //Try to acquire the lock (so wait until the thread really is done
        //pthread_mutex_unlock(&g_threadPool_mutex); //Release the lock
    }
    */

    /*
		while(true) {
        sleep(5000); //5 seconds pause
    }
	*/

    //===================================
    // Exit
    //===================================
    return 0;
}

/*
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
*/

/*
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
*/