#ifndef H_SESSIONMGR
#define H_SESSIONMGR

//Just for the `ACE_HANDLE` declaration that I couldn't find in the ACE documentation
#include "ace/Svc_Handler.h"
//Just for the `ACE_SOCK_STREAM` declaration that I couldn't find in the ACE documentation
#include "ace/SOCK_Stream.h"
//Our session container
#include <map>

class SessionMgr {
public:
	struct s_session {
		//Our socket so others can send us packets
		ACE_SOCK_STREAM socket;
		//Entering world
		bool            logged; //Flag : is the character logged in (selected a character)
		//Character infos
		unsigned int    characterID; //Character ID (not the identity ID)
		std::string     username; //Character in-game name
		BYTE            level; //0 to 255
		bool            gender; //0/false=male || 1/true=female
		unsigned int    map; //Current map
		unsigned int    x; //X position
		unsigned int    y; //Y position
		unsigned int    z; //Z position
		//I'm not scared for all other vars to be defined, because the first one that will be checked
		//is `logged`, hence we perform checks on this only variable first, it would be smart to make
		//sure it have a starting value. Here the user is not logged on by default, so : false 
		s_session() : logged(false) {}
	};
	s_session*	getSession(ACE_HANDLE handle);
	void		addSession(ACE_HANDLE handle);
	void		delSession(ACE_HANDLE handle);
private:
	std::map<ACE_HANDLE,s_session*>	m_sessions;
};

#endif