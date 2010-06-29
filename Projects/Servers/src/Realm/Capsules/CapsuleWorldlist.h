#ifndef H_CAPSULE_WORLDLIST
#define H_CAPSULE_WORLDLIST

//Relative path here; we are in a folder nammed "Casules/".
#include "../Capsule.h"
#include "../SessionMgr.h"
//Virtual path here; these are included by the project settings
#include "database.h"
#include "Packet.h"
#include "resProtocol.h"
#include "resTypedef.h"
//Special includes for handling world server list
#include "../WorldlinkMgr.h"

extern unsigned int g_client_version;
extern database::connection g_db;

class CapsuleWorldlist : public Capsule {
public:
	virtual void doit(SESSION session, Packet& capsule);
}; //end of class

#endif
