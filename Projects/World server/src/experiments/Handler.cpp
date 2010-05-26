#include "Handler.h"

//===============================
//		    CONSTRUCTOR
//===============================
// Once it’s created, a Handler registers itself for READ events with the Dispatcher Singleton.
// Henceforth, when a packet arrives, the Dispatcher automatically dispatches
// the handle_event method of the associated Handler
Handler::Handler(ACE_SOCK_Stream &cs) : m_socket(cs) {
	// Register with the dispatcher for READ events.
	Dispatcher::instance()-> register_handler(this, EVENT_READ);
}

//===============================
//		  HANDLE_EVENT
//===============================
//When a READ event occurs on a socket Handle, the Dispatcher calls back to the
//handle_event method of the Handler. This method receives, processes, and writes the packets.
//Likewise, when the client closes down the connection the Dispatcher passes a CLOSE event,
//which informs the Handler to shut down its SOCK Stream and delete itself.
void Handler::handle_event(Event_Type et) {
	if (et == EVENT_READ) {
		//BYTE buffer here [...]
		//m_socket.recv((void*)log_record, sizeof log_record);
		//Write logging record to standard output.
		//log_record.write(STDOUT);
	}
	else if (et == CLOSE_EVENT) {
		m_socket.close();
		delete (void *)this;
	}
}