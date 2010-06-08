#ifndef H_SESSIONMGR
#define H_SESSIONMGR

//Just for the `ACE_HANDLE` & `stream_type` declaration that I couldn't find in the ACE documentation
#include "ace/Svc_Handler.h"
//Our session container
#include <map>
//Packet buffer
#include "Packet.h"

class SessionMgr {
public:
	struct s_session {
		//Socket buffer
		Packet			buffer;
		//Authentification
		unsigned int    accountID; //Database account ID; needed for SQL queries and stuff like that
		bool            authenticated; //Flag : is the character authenticated (login/password)
		//Information
		bool			isAWorldServer;
		//I'm not scared for all other vars to be defined, because the first one that will be checked
		//is `authenticated`, hence we perform checks on this only variable first, it would be smart to make
		//sure it have a starting value. Here the user is not authenticated by default, so : false
		//Oh and maybe some other flags if needed, let's list them:
		s_session() : authenticated(false), isAWorldServer(false) {}
	};
	s_session*	getSession(ACE_HANDLE handle);
	void		addSession(ACE_HANDLE handle);
	void		delSession(ACE_HANDLE handle);
private:
	std::map<ACE_HANDLE,s_session*>	m_sessions;
};

#endif