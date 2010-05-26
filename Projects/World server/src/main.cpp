#include "main.h"

class TestRunnable : public Thread {
	void run(void) {
		unsigned int counter = 0;
		//--me having some fun with sockets--
		//Socket socket;
		//socket.connect("127.0.0.1", 5000);
		//-----------------------------------
		while (true) {
			std::cerr << "Woohoo, spam from TestRunnable thread :D !" << std::endl;
			//ACE_OS::sleep(ACE_Time_Value(NULL, 1000000)); //1'000'000 = 1 second
			ACE_OS::sleep(1); //This is also 1 second
			//counter++;
			//----- again the socket ----
			/*
			Packet pckt;
			pckt.add<CMD>(PCKT_X_DEBUG);
			pckt.addString("Omg it works!");
			socket.write(pckt);
			*/
			//------------------------------
			//if (counter == 30) break;
		}
	}
};

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

	TestRunnable test;
	test.start();
	
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
	
	// Server port number.
	const u_short port = 6131;
	ACE_INET_Addr server_addr(port);
	
	// Initialize server endpoint an register with the Dispatcher.
	ACE_Acceptor<PacketHandler,ACE_SOCK_ACCEPTOR> acceptor(server_addr, ACE_Reactor::instance(), ACE_NONBLOCK);

	// Main event loop that handles client
	// logging records and connection requests.
	while(true) {
		// dont move this outside the loop, the reactor will modify it
        //ACE_Time_Value interval(0, 100000);

        //if (ACE_Reactor::instance()->run_reactor_event_loop(interval) == -1)
		if (ACE_Reactor::instance()->run_reactor_event_loop() == -1) //when a client d/c it breaks just in case
            break;

		//std::cerr << "Waiting ;o" << std::endl;
		
		//ACE_Reactor::instance()->handle_events(); 
	}

	//ACE_OS::sleep(100);

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
