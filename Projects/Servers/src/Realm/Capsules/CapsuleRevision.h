#ifndef H_CAPSULE_REVISION
#define H_CAPSULE_REVISION

#include "../Capsule.h"
#include "Packet.h"

class CapsuleRevision : public Capsule {
public:
	virtual void doit(Packet& capsule) {
		std::cout << "[CapsuleRevision] Packet received!" << std::endl;
		DWORD revision = capsule.read<DWORD>();
		std::cout << "Revision: " << revision << std::endl;
	}
};

#endif
