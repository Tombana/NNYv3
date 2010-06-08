#include "PacketHandler.h"

PacketHandler::PacketHandler() {
	m_data = new char[BUFFER_SIZE];
}

PacketHandler::~PacketHandler() {
	delete[] m_data;
}

int PacketHandler::open(void*){ 
	//Print a message
	std::cout << "Connection established" << std::endl;
	//Save the handle for now
	ACE_HANDLE handle = get_handle();
	//Print the handle
	std::cout << "Handle: " << handle << std::endl;
	//Add new session to the session manager
	m_sessionMgr.addSession(handle);	
	//Register the service handler with the reactor 
	ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
	//Keep yourself registered with the reactor
	return 0;
}

//After using the peer() method of ACE_Svc_Handler to obtain a reference to the underlying stream of the service
//handler class we call recv() on it to read the data which has been received. This data is stored in the data
//array and then printed out.
//Return  0 = Keep you registered with the reactor
//Return -1 = This will call handle_close() wich close our handle
int PacketHandler::handle_input(ACE_HANDLE handle) {
	//Print the handle
	std::cout << "Handle: " << handle << std::endl;
	
	//Retrieve the session; it is pointer and can be used like the following:
	//session->username = "Blabla";		   //->username is an object of type `std::string`
	//session->inventory.newItem(1234);    //->iventory is an object of type `Inventory`
	SessionMgr::s_session* session = m_sessionMgr.getSession(handle);

	//===== Here the famous packet parser!
	//+++ so all the code following must be put in the section that receive/process incoming packets +++
	//+++ this is usually placed whiting a while(true) loop with a socketRead blocking just before +++

	//===== Input buffer
	//here we have read all bytes from the socket and put it into 'input'
	int readBytes = peer_.recv(m_data,BUFFER_SIZE);
	
	//===== Check if socket isn't closed
	//The recv() function blocks until it receives data.
	//If the function unblock with 0 byte read, the client logged out.
	if (readBytes <= 0) {
			std::cerr << "Client logged out!" << std::endl;
			m_sessionMgr.delSession(handle);
			return -1;
	}

	//'buffer' is a special Packet class to read/write a byte array.
	//This object is unique to all clients and is never destroyed.
	//(well its actually destroyed when the socket disconnect but yeah)
	session->buffer.append(std::string(m_data,readBytes));
	//Print hex packets to help us a little
	session->buffer.printHex();

	//a while() loop here:
	//yeah sometimes there's many capsules in a packet so we have to
	//do this loop more than once, but otherwise (and usually) it will simply break().
	while (true) {
	//----------------------------------
		//Set the buffer seek to the begening so we can perform our checks without errors.
		//By errors I mean this is making sure we arnt lost somewhere else in the buffer
		//like... you guessed it... doing this loop twice.
		//That could totally result in a massive crash if we dont reset the seek pointer.
		session->buffer.setSeek(0); //Here, problem solved, whats next :D

		//Now the seek is placed to the begining, so we are supposed to
		//read a "~" (0x7E) byte firstly.
		if (session->buffer.read<BYTE>() == 0x7E) { //[byte] Packet begining signature
			DWORD length = session->buffer.read<DWORD>(); //we will need this later on so we know when we are done accumulating data

			//Is the length too big? We never know, some hackers could send a fake
			//packet to force the server to save 1Go of garbage in memory.
			//Lets fix a limit of 1MB, i doubt we will somewhen have a packet that long
			//so we are safe here, 1MB is way enough.
			if (length > 1048576) { //1024 Bytes * 1024 KiloBytes = (1 MB) 1048576 Bytes
				//Packet packetToSend;
				//packetToSend.add<CMD>(PCKT_X_HACKING_ATTEMPT);
				//peer_.send_n(session->td.socket << packetToSend;
				//td.socket.socket_close();
				std::cerr << "[packetHandler] @ERROR: Length refused!" << std::endl;
				std::cerr << "[PacketHandler] @ERROR: Socket closed for security reasons." << std::endl;
				m_sessionMgr.delSession(handle);
				return -1; //break ACE_Svn_Handle and close the socket
			}

			//Okay now the interesting part:
			//is the [buffer data] (full size - packet header)
			//greater OR equal the [capsule data] length we have been told earlier?
			if (session->buffer.size()-5 >= length) {
				//Awesome we have enough bytes now :D
				//Lets continue the "so waited" code part
				//Copy the 5+length first bytes from the buffer to 'capsule'.
				Packet capsule;
				capsule.append(session->buffer.getStringRaw().substr(5, length)); //position to 5 and read length bytes
				//-----------------------------------------------
				//PROCESS (DISPTACH IN OUR CASE) THE CAPSULE
				//PACKETDISPATCHER::instance()->dispatch(packetCmd);
				//-----------------------------------------------
				//Delete the 5+length first bytes from the buffer
				session->buffer.erase(0, 5+length); //position to 0 and delete 5+length character
				//We already processed it, we no need anymore. Anyway keeping it
				//would totally screw up the code because the Seek is always set
				//to the begening remember? So trust me, delete it >:D

				//Oh well, whats next, the buffer might have move capsules in it
				//(except the one we just processed and deleted)
				//so, easy solution, lets do the loop one more time :P
				//But before looping, lets check if I was right
				if (session->buffer.size() == 0) {
					//We have read all the capsules in this packet
					//Eheh, oh well we dont need to loop :)
					break; //break the processing loop to wait for another packet from the socket
				} else {
					//Eheh, i was right, there is really is another capsule in this packet :D
				}
			} else {
				//The buffer doesnt have enough bytes yet,
				//we still need to append new inputs to the buffer :(
				//So lets break for now to read another packet from the socket
				//and... well... continue accumulating
				//until we have enough bytes to process that damn packet :D
				break; //break the processing loop to wait for another packet from the socket
			}

		} else {
			//At this point, i hope we arent expecting a new capsule
			//because if its the case, that capsule doesnt starts with 0x7E.
			//Something is definitivelly going wrong.
			std::cerr << "[PacketHandler] @ERROR: Packets are corrupted!" << std::endl;
			std::cerr << "[PacketHandler] @ERROR: There is another capsule in this packet, not starting with 0x7E!" << std::endl;
			std::cerr << "[PacketHandler] @ERROR: Socket closed for security reasons." << std::endl;
			m_sessionMgr.delSession(handle);
			return -1; //break ACE_Svc_Handler, close the socket as well
		}
	}

	return 0; 
 }