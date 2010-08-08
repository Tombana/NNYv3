#ifndef H_ABSTRACT_CAPSULE
#define H_ABSTRACT_CAPSULE

#include "Packet.h"

///Abstract template class and functionoid.
template <typename T_SESSION>
class Capsule {
public:
	virtual void doit(T_SESSION session, Packet& capsule) = 0;
};

#endif