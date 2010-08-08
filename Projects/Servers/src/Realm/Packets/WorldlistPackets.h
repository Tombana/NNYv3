#ifndef H_WORLDLISTPACKETS
#define H_WORLDLISTPACKETS

//STD library
#include <iostream>
//Needed parent folder files
#include "../session.h"
#include "../WorldlinkMgr.h"

class WorldlistPackets {
public:
	static void getWorldlist(SESSION& session);
};

#endif