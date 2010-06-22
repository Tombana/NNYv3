#ifndef H_SESSIONMGR
#define H_SESSIONMGR

//Just for the `ACE_HANDLE` & `stream_type` declaration that I couldn't find in the ACE documentation
#include "ace/Svc_Handler.h"
//Even more
#include "ace/SOCK_Stream.h"
#include "ace/Singleton.h"
//Our session container
#include <map>
//Socket & packet buffer
#include "Packet.h"
#include "Socket.h"

/**
* The SessionMgr store sessions in memory and provide access to it when needed with a pointer.
* @class SessionMgr
* @brief User's session manager
* @note This class is a singleton and so can only be used through calls like this: SESSIONMGR::instance()->getSession(ACE_HANDLE);
* @note The container used is std::map
**/
class SessionMgr {
	///This make our class a singleton; read notes below if you need help using it.
	friend class ACE_Singleton<SessionMgr, ACE_Null_Mutex>;
public:
	///This is the structure created in memory.\n It contains all users informations the server could ever need to save at some point
	struct s_session {
		//Socket & packet buffer
		Packet			buffer;
		Socket			socket;
		//Information
		bool			isAWorldServer;
		//I'm not scared for all other vars to be defined, because the first one that will be checked
		//is `authenticated`, hence we perform checks on this only variable first, it would be smart to make
		//sure it have a starting value. Here the user is not authenticated by default, so : false
		//Oh and maybe some other flags if needed, let's list them:
		s_session() : isAWorldServer(false) {}
	};
	/**
	* @brief Returns a pointer to the s_session structure stored in memory.
	* @param[in] handle is used as the array key for std::map
	*/
	s_session*	getSession(ACE_HANDLE handle);
	/**
	* @brief Create a new session in memory and map it.
	* @param[in] handle will be used as the array key by std::map
	*/
	void		addSession(ACE_HANDLE handle);
	/**
	* @brief Delete a s_session structure from memory & un-map it.
	* @param[in] handle is used as the array key for std::map
	*/
	void		delSession(ACE_HANDLE handle);
	
private:
	///This is the container. It shouldn't get that big because they are only pointers.
	std::map<ACE_HANDLE,s_session*>	m_sessions;
	//contructor/destructor are privates, so the class can only be constructed with the ACE_Singleton typedef define below (SESSIONMGR)
	SessionMgr();
	~SessionMgr();
};

typedef ACE_Singleton<SessionMgr, ACE_Null_Mutex> SESSIONMGR;
typedef	SessionMgr::s_session*					  SESSION;

#endif