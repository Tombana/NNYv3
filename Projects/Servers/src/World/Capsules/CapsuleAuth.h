#ifndef H_CAPSULE_AUTH
#define H_CAPSULE_AUTH

#include <iostream>
#include <string>
#include <algorithm>
#include "Capsule.h"
#include "Packet.h"
#include "database.h"

#include "resProtocol.h"
#include "resTypedef.h"

#include "../session.h" //define s_session

extern database::connection g_db;

class CapsuleAuth : public Capsule<s_session*> {
public:
	void doit(s_session* session, Packet& capsule);
};

#endif
