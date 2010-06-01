#ifndef H_EVENTHANDLER
#define H_EVENTHANDLER

#include "sharedModels.h"
#include "ace/Event_Handler.h" //for ACE_HANDLE so far

// Abstract base class that serves as the target of the Dispatcher.
class Event_Handler {
public:
// Hook method that is called back by the Dispatcher to handle events.
virtual void handle_event(Event_Type et);
// Hook method that returns the underlying I/O Handle.
virtual ACE_HANDLE get_handle(void) const = 0;
};

/*
class Event_Handler
{
public:
// Hook methods that are called back by the Dispatcher to handle particular types of events.
virtual int handle_accept (void) = 0;
virtual int handle_input (void) = 0;
virtual int handle_output (void) = 0;
virtual int handle_timeout (void) = 0;
virtual int handle_close (void) = 0;
// Hook method that returns the underlying I/O Handle.
virtual Handle get_handle (void) const = 0;
};
*/

//Note: There's no .cpp file along this header file.

#endif