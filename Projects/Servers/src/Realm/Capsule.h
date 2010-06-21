#ifndef H_CAPSULE
#define H_CAPSULE

#include "Packet.h"
#include "SessionMgr.h"

///Abstract class and functionoid.
class Capsule {
public:
	virtual void doit(SESSION session, Packet& capsule) = 0;
};

#endif