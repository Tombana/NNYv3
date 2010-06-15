#include "main.h"

//Obviously, this is the main program.
//At first it prints the welcome screen, bind the server port and start the ACE_Reator coupled with ACE_Acceptor.
//Then it will call ACE_Reactor::run_reactor_event_loop() in a infinite loop (it breaks if ACE returns any error).

int main(int argc, char **argv) {
	//=========================================
    // STARTUP MESSAGE
    //=========================================
    std::cout << "NN      NN  NN      NN NN    NN             "   << std::endl;
    std::cout << "NNNN    NN  NNNN    NN  NN  NN        NNNNN "   << std::endl;
    std::cout << "NN NN   NN  NN NN   NN   NNNN            NN "   << std::endl;
    std::cout << "NN  NN  NN  NN  NN  NN    NN  NN    NN  NN  "   << std::endl;
    std::cout << "NN   NN NN  NN   NN NN    NN  NN   NN  NNN  "   << std::endl;
	std::cout << "NN    NNNN  NN    NNNN    NN   NN NN     NN "   << std::endl;
	std::cout << "NN     NNN  NN     NNN    NN    NNN   NNNN  "   << std::endl;
	std::cout << "-------------------------------"				  << std::endl;
	std::cout << "- Type: Realm server"							  << std::endl;
	std::cout << "- Protocol version: " << NNY_PROTOCOL_VERSION   << std::endl;
    //std::cout << "- Config filename: " << CONFIG_FILENAME		  << std::endl;
	std::cout << "-------------------------------"				  << std::endl;
	
	//=========================================
    // SETTING UP : `ACE_Acceptor`
    //=========================================
    std::cout << "Setting up acceptor..." << std::endl;
	
	// Server port number.
	const u_short port = 6131;
	ACE_INET_Addr server_addr(port);
	
	// Initialize server endpoint an register with the Reactor.
	ACE_Acceptor<PacketHandler,ACE_SOCK_ACCEPTOR> acceptor(server_addr, ACE_Reactor::instance(), ACE_NONBLOCK);

	//=========================================
    // MAIN PROGRAM LOOP : `ACE_Reactor`
    // It handles ACE_Acceptor events (like connections/deconnections) and incoming packets.
    //=========================================
	
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
    // EXIT
    //===================================
    return 0;
}