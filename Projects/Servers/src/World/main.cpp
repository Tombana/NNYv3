#include "main.h"

/*
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
			Packet pckt;
			pckt.add<CMD>(PCKT_X_DEBUG);
			pckt.addString("Omg it works!");
			socket.write(pckt);
			//------------------------------
			//if (counter == 30) break;
		}
	}
};
*/

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
	std::cout << "-------------------------------"				  << std::endl;
	std::cout << "- Type: World server"							  << std::endl;
	std::cout << "- Protocol version: " << NNY_PROTOCOL_VERSION   << std::endl;
    //std::cout << "- Config filename: " << CONFIG_FILENAME		  << std::endl;
	std::cout << "-------------------------------"				  << std::endl;
	
	//TestRunnable test;
	//test.start();
	
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
    std::cout << "Setting up acceptor..." << std::endl;
	
	// Server port number.
	const u_short port = 6131;
	ACE_INET_Addr server_addr(port);
	
	// Initialize server endpoint an register with the Reactor.
	ACE_Acceptor<PacketHandler,ACE_SOCK_ACCEPTOR> acceptor(server_addr, ACE_Reactor::instance(), ACE_NONBLOCK);

	// Main event loop that handles packets
	std::cout << "Server is now running!" << std::endl;
	while(true) {
		// dont move this outside the loop, the reactor will modify it
        //ACE_Time_Value interval(0, 100000);

		//ACE_Reactor::instance()->handle_events();
        //if (ACE_Reactor::instance()->run_reactor_event_loop(interval) == -1)
		if (ACE_Reactor::instance()->run_reactor_event_loop() == -1) {
			std::cout << "[Debug] Reactor event loop broken! What happened?!" << std::endl;
			break;
		}
	}

    //===================================
    // Exit
    //===================================
    return 0;
}

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
