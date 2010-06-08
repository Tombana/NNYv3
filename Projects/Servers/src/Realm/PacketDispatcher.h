#ifndef H_PACKETDISPATCHER
#define H_PACKETDISPATCHER

//Includes
#include "ace/Singleton.h" //PacketDispatcher is a singleton; only one instance possible
#include <iostream>		   //Useful to display errors if any
#include "resTypedef.h"    //dispatch() parameter is 'CMD' type; defined in this file
#include "resProtocol.h"   //Needed to setup the table of interest
#include "Packet.h"

//We are dealing with functionoids/capsule stuff here
#include "Capsule.h"
//Needed to setup the table of interest
#include "Capsules/CapsuleRevision.h"

//Note: This class is a singleton and so can only be used through calls like this:
//PACKETDISPATCHER::instance()->dispatch(CMD);
class PacketDispatcher {
	friend class ACE_Singleton<PacketDispatcher, ACE_Null_Mutex>;
	public:
		void dispatch(CMD packetCmd, Packet &capsule);
	private:
		//Member variables
		Capsule* m_table[NNY_PROTOCOL_RANGE];
		//contructor/destructor are privates, so the class can only be constructed
		//with the ACE_Singleton typedef just below: PACKETDISPATCHER
		PacketDispatcher();
		~PacketDispatcher();
};

typedef ACE_Singleton<PacketDispatcher, ACE_Null_Mutex> PACKETDISPATCHER;

#endif