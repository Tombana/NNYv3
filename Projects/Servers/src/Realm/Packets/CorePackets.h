#ifndef H_CORE_PACKETS
#define H_CORE_PACKETS

//STD library
#include <iostream>
//Relative `Common` & `Resource` files
#include "Packet.h"

class CorePackets {
public:
	static void debug(Packet& input);
	static void invalidCMD(Packet& input);
};

#endif