#include "PacketDispatcher.h"

void PacketDispatcher::dispatch(SESSION& session, Packet& input) {
	//I used to deal with arrays of function pointers but I just learned the compiler will optimize
	//this switch to be as fast as possible. The compiler is always right and if you think you can do
	//better than theses guys, I'd really suggest you to quit every projects you are working on,
	//share your new discovery and get yourself known.
	//Here we are aiming for a switch jump table but again it's according to your compiler capability.
	//Please note the cases must be as narrow as possible with only few gasp,
	//otherwise the compiler might consider the trade-off and use another method, such as:
	//Binary search, any fancy tree or simply bunch of if/elseif/else statements.
	switch (input.read<CMD>()) {	//TODO: Sort this switch case.
		case PCKT_R_WELCOME:		RealmPackets::welcome(session); break;
		case PCKT_X_DEBUG:			CorePackets::debug(input); break;
		case PCKT_R_SYNC_KEY_ACK:	RealmPackets::syncKeyACK(input); break;
		case PCKT_C_AUTH:			ConnectionPackets::auth(session, input); break;
		//case PCKT_C_GETCHARLIST:	ConnectionPackets::getCharList(..); break;
		//case PCKT_C_DELETECHAR:	ConnectionPackets::deleteChar(..); break;
		//case PCKT_C_ENTER_WORLD:	ConnectionPackets::enterWorld(..); break;
		//case PCKT_C_MOVE_XYZ:		GamePackets::moveXYZ(..);
		//case PCKT_C_MOVE_WASD:	GamePackets::moveWASD(..);
		//case PCKT_C_JUMP:			GamePackets::jump(..);
		//case PCKT_C_FOLLOW:		GamePackets::follow(..);
		//case PCKT_C_USE_SKILL:	GamePackets::useSkill(..);
		//case PCKT_C_USE_ITEM:		GamePackets::useItem(..);
		//case PCKT_C_TRADE:		GamePackets::trade(..);
		//case PCKT_C_ITEM_PICKUP:	GamePackets::pickupItem(..);
		//case PCKT_C_ITEM_DROP:	GamePackets::dropItem(..);
		//case PCKT_C_ITEM_DESTROY: GamePackets::destroyItem(..);
		//case PCKT_C_BANK_ADD:		GamePackets::addBankItem(..);
		//case PCKT_C_BANK_REMOVE:	GamePackets::removeBankItem(..);
		//case PCKT_C_BANK_MOVE:	GamePackets::moveBankItem(..);
		default:					CorePackets::invalidCMD(input);
	}
}