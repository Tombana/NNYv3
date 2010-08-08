#include "RealmConnector.h"

RealmConnector::RealmConnector() {
	m_errorCounter = 0;
}

RealmConnector::~RealmConnector() {
}

void RealmConnector::run() {
	forever {
        if (m_socket.connect("127.0.0.1", 6131)) { //TODO: Load from configuration file
			//Get handle
			ACE_HANDLE handle = m_socket.getHandle();
			//Add new session to the session manager
			SESSIONMGR::instance()->addSession(handle);
			//Save the session for now
			SESSION session = SESSIONMGR::instance()->getSession(handle);
			//Set the session infos
			session->socket = m_socket;
			session->isARealmServer = true;
			//Register the socket to the reactor
			ACE_Reactor::instance()->register_handler(handle, PACKETHANDLER::instance(), ACE_Event_Handler::READ_MASK);
			//Pause the thread
			suspend();
		} else {
			//Is the error counter to the maximum retries possible
			if (m_errorCounter >= CONFIG_REALMCONNECTOR_MAX_RETRY) {
				//If so, stop asking the realm and take a little nap
				ACE_OS::sleep(ACE_Time_Value(CONFIG_REALMCONNECTOR_SLEEPTIME)); //in seconds
				//Reset the counter so it retry again multiple times before taking another nap
				m_errorCounter=0;
			} else {
				m_errorCounter++;
			}
		}
		//Just in case
		m_socket.close();
    }
}