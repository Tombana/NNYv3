#ifndef H_CAPSULE_DEBUG
#define H_CAPSULE_DEBUG

#include "Capsule.h"
#include "Packet.h"

#include "../session.h" //define s_session

class CapsuleDebug : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
};

#endif
