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
#include "Capsules/CapsuleVersion.h"
#include "Capsules/CapsuleDebug.h"

/**
* @brief Once a capsule is received, this will perform a quick check and dispatch it to the right capsule functionoid.
* @class PacketDispatcher
* @note This class is a singleton and so can only be used through calls like this: PACKETDISPATCHER::instance()->dispatch(CMD,Packet);
**/
class PacketDispatcher {
	///This make our class a singleton; read the notes below if you need help using it.
	friend class ACE_Singleton<PacketDispatcher, ACE_Null_Mutex>;
	
	public:
		///Check the @a packetCmd in @a capsule and dispatch it to the right functionoid using @a m_table.
		void dispatch(CMD packetCmd, Packet &capsule);
	
	private:
		///An array of pointers to all capsule functionoids
		Capsule* m_table[NNY_PROTOCOL_RANGE];
		//contructor/destructor are privates, so the class can only be constructed with the ACE_Singleton typedef define below (PACKETDISPATCHER)
		///Create and fill the @a m_table array with @a packetCmd@zs were are interested to (hard-coded).
		PacketDispatcher();
		///Free and delete the @a m_table array.
		~PacketDispatcher();
};

typedef ACE_Singleton<PacketDispatcher, ACE_Null_Mutex> PACKETDISPATCHER;

#endif
