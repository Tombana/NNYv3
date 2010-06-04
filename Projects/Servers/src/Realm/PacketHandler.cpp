#include "PacketHandler.h"

PacketHandler::PacketHandler() {
	m_data = new char[BUFFER_SIZE];
}

PacketHandler::~PacketHandler() {
	delete[] m_data;
}

int PacketHandler::open(void*){ 
	std::cout << "Connection established" << std::endl;
	std::cout << "Connection handle: " << get_handle() << std::endl;
	unsigned int handleID = (unsigned int)get_handle() / sizeof(ACE_HANDLE);
	std::cout << "Connection handleID ("<<get_handle()<<"/"<<sizeof(ACE_HANDLE)<< "): " << handleID << std::endl;
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

	std::cout << "Handle: " << handle << std::endl;
	int readBytes = peer().recv(m_data,BUFFER_SIZE);

	std::string tempInput(m_data, readBytes);
	std::cout << "Input: " << tempInput << std::endl;

	if (readBytes == 0) {
		std::cerr << "Client logged out!" << std::endl;
		return -1;
	}
	
	return 0; 
 } 