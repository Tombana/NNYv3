#ifndef H_CAPSULE_DEBUG
#define H_CAPSULE_DEBUG

#include "../Capsule.h"
#include "Packet.h"

class CapsuleDebug : public Capsule {
public:
	virtual void doit(SESSION session, Packet& capsule);
};

#endif
