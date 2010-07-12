#ifndef H_SESSIONMGR
#define H_SESSIONMGR

//Just for the `ACE_HANDLE` & `stream_type` declaration that I couldn't find in the ACE documentation
#include "ace/Svc_Handler.h"
//Even more
#include "ace/SOCK_Stream.h"
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
//Our session container
#include <map>
//Socket & packet buffer
#include "Packet.h"
#include "Socket.h"

/**
* The SessionMgr store sessions in memory and provide access to it when needed with a pointer.
* @class SessionMgr
* @brief User's session manager
* @note This class is a singleton and so can only be used through calls like this: ACE_Singleton<SessionMgr<T_SESSION>,ACE_Null_Mutex>::instance()->getSession(ACE_HANDLE);
* @note The container used is std::map
**/
template <typename T_SESSION>
class SessionMgr {
	///This make our class a singleton; read notes below if you need help using it.
	friend class ACE_Singleton<SessionMgr<T_SESSION>, ACE_Null_Mutex>;
public:
	/**
	* @brief Returns a pointer to the s_session structure stored in memory.
	* @param[in] handle is used as the array key for std::map
	*/
	T_SESSION getSession(ACE_HANDLE handle) {
		std::map<ACE_HANDLE,T_SESSION>::const_iterator iterator = m_sessions.find(handle);
		if (iterator == m_sessions.end()) {
			return NULL; //return NULL
		} else {
			return iterator->second; //return T_SESSION
		}
	}
	/**
	* @brief Create a new session in memory and map it.
	* @param[in] handle will be used as the array key by std::map
	*/
	void	addSession(ACE_HANDLE handle) {
		//Create a new session structure in memory then assign it with std::map.
		m_sessions[handle] = new s_session;
	}
	/**
	* @brief Delete a s_session structure from memory & un-map it.
	* @param[in] handle is used as the array key for std::map
	*/
	void	delSession(ACE_HANDLE handle) {
		delete m_sessions[handle]; //Free the s_session from memory
		m_sessions.erase(handle); //Delete the session pointer from std::map
	}
	
private:
	///This is the container. It shouldn't get that big because they are only pointers.
	std::map<ACE_HANDLE,T_SESSION>	m_sessions;
	//contructor/destructor are privates, so the class can only be constructed with the ACE_Singleton typedef define below (ASessionMgr)
	SessionMgr() {}
	~SessionMgr() {}
};

#endif