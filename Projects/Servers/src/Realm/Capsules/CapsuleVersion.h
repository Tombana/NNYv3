#ifndef H_CAPSULE_VERSION
#define H_CAPSULE_VERSION

//Common files
#include "Capsule.h"
#include "database.h"
#include "Packet.h"
#include "resProtocol.h"
//Includes
#include "../session.h" //define s_session

extern unsigned int g_client_version;
extern database::connection g_db;

class CapsuleVersion : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
}; //end of class

#endif
