#ifndef H_CAPSULE_VERSION
#define H_CAPSULE_VERSION

#include "../Capsule.h"
#include "Packet.h"

class CapsuleVersion : public Capsule {
public:
	virtual void doit(Packet& capsule) {
		std::cout << "[CapsuleVersion] Packet received!" << std::endl;
		DWORD version = capsule.read<DWORD>();
		std::cout << "Version: " << version << std::endl;
	}
};

#endif
