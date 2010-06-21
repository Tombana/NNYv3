#ifndef H_CAPSULE_DEBUG
#define H_CAPSULE_DEBUG

#include "../Capsule.h"
#include "Packet.h"

class CapsuleDebug : public Capsule {
public:
	virtual void doit(SESSION session, Packet& capsule) {
		std::cout << "[CapsuleDebug] Packet received!" << std::endl;
		std::cout << "Message: " << capsule.readString() << std::endl;
	}
};

#endif
