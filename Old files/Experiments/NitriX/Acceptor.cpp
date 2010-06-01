#include "Acceptor.h"

Acceptor::Acceptor (const ACE_INET_Addr &addr) : m_acceptor(addr) {
	// Register acceptor with the Dispatcher, which "double dispatches"
	// the Acceptor::get_handle() method to obtain the HANDLE.
	Dispatcher::instance()->register_handler(this, EVENT_ACCEPT);
	// Henceforth, whenever a client connection arrives,
	// the Dispatcher calls back to the Acceptor’s handle_event method as shown below
}

// The Acceptor can only be called for an ACCEPT event.
void Acceptor::handle_event(Event_Type et) {
assert(et == EVENT_ACCEPT);
ACE_SOCK_Stream newConnection;
// Accept the connection.
m_acceptor.accept(newConnection);
// Create a new Handler.
Handler *handler = new Handler(newConnection);
}