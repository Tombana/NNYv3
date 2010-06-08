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
	//Retrieve the session; it is pointer and can be used like the following:
	//session->username = "Blabla";		   //->username is an object of type `std::string`
	//session->inventory.newItem(1234);    //->iventory is an object of type `Inventory`
	SessionMgr::s_session* session = m_sessionMgr.getSession(handle);

	//Print the handle
	std::cout << "Handle: " << handle << std::endl;
	
	//Receive packets and print the input
	int readBytes = peer_.recv(m_data,BUFFER_SIZE);
	std::string tempInput(m_data, readBytes);
	std::cout << "Input: " << tempInput << std::endl;

	//The recv() function blocks until it have received data.
	//If the function unblock with 0 byte read, the client logged out.
	if (readBytes == 0) {
		std::cerr << "Client logged out!" << std::endl;
		return -1;
	} else {
		//Dispatch packet
		//PACKETDISPATCHER::instance()->dispatch(packetCmd);
	}
	
	return 0; 
 }