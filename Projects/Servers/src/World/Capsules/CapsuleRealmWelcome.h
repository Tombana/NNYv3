#ifndef H_CAPSULE_REALMWELCOME
#define H_CAPSULE_REALMWELCOME

#include <iostream>
#include "Capsule.h"
#include "Packet.h"
#include "resProtocol.h"

#include "../session.h" //define s_session

class CapsuleRealmWelcome : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
};

#endif