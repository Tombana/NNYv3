#include "SessionMgr.h"

SessionMgr::s_session*	SessionMgr::getSession(ACE_HANDLE handle) {
	return m_sessions[handle]; //std::map return a pointer?
}

void SessionMgr::addSession(ACE_HANDLE handle) {
	//Create a new session structure in memory then assign a placeholder in std::map with it.
	m_sessions[handle] = new s_session;
}

void SessionMgr::delSession(ACE_HANDLE handle) {
	delete m_sessions[handle]; //Delete the session from memory
	m_sessions.erase(handle); //Delete the pointer from the std::map
}