#ifndef H_PACKETDISPATCHER
#define H_PACKETDISPATCHER

//Includes
#include "ace/Singleton.h" //PacketDispatcher is a singleton; only one instance possible
#include "ace/Null_Mutex.h"
#include <iostream>		   //Useful to display errors if any
#include "resTypedef.h"    //dispatch() parameter is 'CMD' type; defined in this file
#include "resProtocol.h"   //Needed to setup the table of interest
#include "Packet.h"
#include "SessionMgr.h"
#include "Capsule.h"

/**
* @brief Once a capsule is received, this will perform a quick check and dispatch it to the right capsule functionoid.
* @class PacketDispatcher
* @note This class is a singleton and so can only be used through calls like this: ACE_Singleton<PacketDispatcher<T_SESSION>,ACE_Null_Mutex>::instance()->dispatch(CMD,Packet);
**/
template <typename T_SESSION>
class PacketDispatcher {
	///This make our class a singleton; read the notes below if you need help using it.
	friend class ACE_Singleton<PacketDispatcher<T_SESSION>, ACE_Null_Mutex>;
	
	public:
		///Check the @a packetCmd in @a capsule and dispatch it to the right functionoid using @a m_table.
		void dispatch(T_SESSION session, CMD packetCmd, Packet &capsule) {
			//Check if the packetCmd really exist
			//Must be between 0(excluded) and lower to NNY_PROTOCOL_RANGE(included)
			//Notation: ]0,NNY_PROTOCOL_RANGE]
			if (packetCmd <= NNY_PROTOCOL_RANGE && packetCmd > 0) {
				//Check if it have a functionoid assigned ('else' if NULL)
				if (m_table[packetCmd]) {
					m_table[packetCmd]->doit(session, capsule);
				} else {
					std::cout << "[PacketDispatcher::dispatch] Packet & socket dropped; not set in table of interest!" << std::endl;
					capsule.setSeek(capsule.size());
					//Close socket
					//session->socket.close();
					//TODO: Free the session?
				}
			} else {
				std::cout << "[PacketDispatcher::dispatch] Packet & socket dropped; out of protocol range!" << std::endl;
				capsule.setSeek(capsule.size());
				//Close socket
				//session->socket.close();
				//TODO: Free the session?
			}
		}
		//Setting up table of interest
		void addSource(CMD packetCmd, Capsule<T_SESSION>* capsule) {
			m_table[packetCmd] = capsule;
		}

	private:
		///An array of pointers to all capsule functionoids
		Capsule<T_SESSION>* m_table[NNY_PROTOCOL_RANGE];
		//contructor/destructor are privates, so the class can only be constructed with the ACE_Singleton typedef define below (PacketDispatcher)
		///Create and fill the @a m_table array with @a packetCmd@zs were are interested to (hard-coded).
		PacketDispatcher() {
			//Set the whole array set to NULL; just in case.
			//Note that it's impossible to initialize an array 
			//in a class's initialization list or as static/const.
			for(unsigned int i=0; i<=NNY_PROTOCOL_RANGE; i++)
				m_table[i] = NULL;
		}
		///Free and delete the @a m_table array.
		~PacketDispatcher() {
			//Free memory of array objects; note delete NULL won't crash
			//If the argument is 0, the delete expression has no effect. In other words,
			//deleting a null pointer is a safe operation. There is no need to check the
			//pointer for null-ness before passing it to delete:
			for(unsigned int i=0; i<=NNY_PROTOCOL_RANGE; i++)
				delete m_table[i];
		}
};

#endif
