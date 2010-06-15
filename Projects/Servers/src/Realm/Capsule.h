#ifndef H_CAPSULE
#define H_CAPSULE

#include "Packet.h"

///Abstract class and functionoid.
class Capsule {
public:
	virtual void doit(Packet& capsule) = 0;
};

#endif