#ifndef H_DISPATCHER
#define H_DISPATCHER

#include "ace/Time_Value.h"
#include "EventHandler.h"

// Demultiplex and dispatch Event_Handlers in response to client requests.
class Dispatcher {
	// Register an Event_Handler of a particular Event_Type
	// (e.g., EVENT_READ, EVENT_ACCEPT, etc.).
	int register_handler(Event_Handler *eh, Event_Type et);
	// Remove an Event_Handler of a particular Event_Type.
	int remove_handler(Event_Handler *eh, Event_Type et);
	// Entry point into the reactive event loop.
	int handle_events(ACE_Time_Value *timeout = 0);
};

#endif