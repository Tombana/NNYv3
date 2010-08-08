#include "CorePackets.h"

void CorePackets::debug(Packet& input) {
	std::cout << "[CorePackets][debug] PCKT_X_DEBUG received!" << std::endl;
	std::cout << "Message: " << input.readString() << std::endl;
}

void CorePackets::invalidCMD(Packet &input) {
	input.backSeek(sizeof(CMD));
	std::cout << "[CorePackets][invalidCMD] Packet #" << input.read<CMD>() << " doesn't exist!" << std::endl;
	std::cout << "Dump: (";
	input.printHexFromSeek();
	std::cout << ")" << std::endl;
	input.clear();
}