#ifndef H_CAPSULE_WORLDLIST
#define H_CAPSULE_WORLDLIST

//Common files
#include "Capsule.h"
#include "database.h"
#include "Packet.h"
#include "resProtocol.h"
#include "resTypedef.h"
//Special includes for handling world server list
#include "../WorldlinkMgr.h"
//Includes
#include "../session.h" //define s_session

extern unsigned int g_client_version;
extern database::connection g_db;

class CapsuleWorldlist : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
}; //end of class

#endif
