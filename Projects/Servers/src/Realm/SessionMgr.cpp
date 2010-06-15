#include "SessionMgr.h"

SessionMgr::SessionMgr() {}

SessionMgr::~SessionMgr() {}

SessionMgr::s_session*	SessionMgr::getSession(ACE_HANDLE handle) {
	return m_sessions[handle]; //std::map objects are s_session* already, no need type conversion
}

void SessionMgr::addSession(ACE_HANDLE handle) {
	//Create a new session structure in memory then assign it with std::map.
	m_sessions[handle] = new s_session;
}

void SessionMgr::delSession(ACE_HANDLE handle) {
	delete m_sessions[handle]; //Free the s_session from memory
	m_sessions.erase(handle); //Delete the session pointer from std::map
}