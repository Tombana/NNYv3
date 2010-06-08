#ifndef H_FUNCTIONOID_REVISION
#define H_FUNCTIONOID_REVISION

#include "Functionoid.h"

class FunctionoidRevision : public Functionoid {
public:
	virtual void doit() {
		std::cout << "[FunctionoidRevision] Packet received!" << std::endl;
	}
};

#endif