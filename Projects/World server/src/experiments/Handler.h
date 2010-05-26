#ifndef H_HANDLER
#define H_HANDLER

#include "EventHandler.h"
#include "sharedModels.h"
#include "ace/SOCK_Stream.h"
#include "Dispatcher.h"

// Receive and process packets sent by a client application.
class Handler : public Event_Handler {
public:
// Initialize the client stream.
Handler (ACE_SOCK_Stream &cs);
// Hook method that handles the reception of logging records from clients.
virtual void handle_event(Event_Type et);
// Get the I/O Handle (called by the Dispatcher when Handler is registered).
virtual ACE_HANDLE get_handle(void) const {
return m_socket.get_handle();
}
private:
// Receives packets from a client.
ACE_SOCK_Stream m_socket;
};

#endif