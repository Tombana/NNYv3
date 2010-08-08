#ifndef H_SESSION
#define H_SESSION

#include "Packet.h"
#include "Socket.h"
#include "resTypedef.h"

struct s_session {
	//Socket & packet buffer
	Packet			buffer;
	Socket			socket;
	//Information
	bool			isAWorldServer;
	DWORD			worldID;
	//I'm not scared for all other vars to be defined, because the first one that will be checked
	//is `authenticated`, hence we perform checks on this only variable first, it would be smart to make
	//sure it have a starting value. Here the user is not authenticated by default, so : false
	//Oh and maybe some other flags if needed, let's list them:
	s_session() : isAWorldServer(false) {}
};

typedef s_session* SESSION;

#endif