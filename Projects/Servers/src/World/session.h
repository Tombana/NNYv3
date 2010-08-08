#ifndef H_SESSION
#define H_SESSION

#include "Packet.h"
#include "Socket.h"

struct s_session {
	//Socket & packet buffer
	Packet			buffer;
	Socket			socket;
	//Realm server exchange
	bool			isARealmServer;
	//Defaut constructor
	s_session() : isARealmServer(false) {}
};

typedef	s_session* SESSION;

#endif