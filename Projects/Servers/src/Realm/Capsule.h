#ifndef H_CAPSULE
#define H_CAPSULE

#include "Packet.h"

class Capsule {
public:
	virtual void doit(Packet& capsule) = 0;
};

#endif