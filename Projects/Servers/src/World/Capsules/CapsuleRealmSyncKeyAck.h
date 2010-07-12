#ifndef H_CAPSULE_REALM_SYNC_KEY_ACK
#define H_CAPSULE_REALM_SYNC_KEY_ACK

#include <iostream>
#include "Capsule.h"
#include "Packet.h"
#include "resProtocol.h" //ACK_SUCCESS definition
#include "resTypedef.h" //ACK type

#include "../session.h" //define s_session

class CapsuleRealmSyncKeyAck : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
};

#endif