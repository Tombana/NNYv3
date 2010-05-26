#ifndef H_ACCEPTOR
#define H_ACCEPTOR

#include "sharedModels.h"
#include "EventHandler.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Acceptor.h"
#include "Dispatcher.h"
#include "Handler.h"

// Handles client connection requests.
class Acceptor : public Event_Handler {
public:
// Initialize the m_acceptor endpoint and register with the Dispatcher.
Acceptor(const ACE_INET_Addr &addr);
// Factory method that accepts a new SOCK_Stream connection and creates a
// Handler object to handle logging records sent using the connection.
virtual void handle_event(Event_Type et);
// Get the I/O Handle (called by the Dispatcher when Acceptor is registered).
virtual ACE_HANDLE get_handle(void) const {
return m_acceptor.get_handle();
}
private:
// Socket factory that accepts client connections.
ACE_SOCK_Acceptor m_acceptor;
};

#endif