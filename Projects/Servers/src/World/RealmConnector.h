#ifndef H_REALMCONNECTOR
#define H_REALMCONNECTOR

#include "ace/Reactor.h" //It register to the Reactor
#include "ace/Event_Handler.h" //For the mask definition
#include "ace/Singleton.h" //RealmConnector is a singleton
#include "Thread.h" //RealmConnector is also a thread
#include "Socket.h" //It uses sockets to contact to Realm server
#include "config.h" //Let the user cuztomise what is possible to cuztomise
#include "PacketHandler.h" //Needed to register our socket to the packet handler

#include "SessionMgr.h"
#include "session.h"
typedef	s_session* SESSION;
typedef ACE_Singleton<SessionMgr<SESSION>,ACE_Null_Mutex> SESSIONMGR;
typedef ACE_Singleton<PacketHandler,ACE_Null_Mutex> PACKETHANDLER;

class RealmConnector : public Thread {
	friend class ACE_Singleton<RealmConnector, ACE_Null_Mutex>;
	public:
		void			run();
		/*
		inline Socket*  getRealmSocket() {
			return &m_socket;
		}
		*/
	private:
		Socket			m_socket;
		unsigned short  m_errorCounter; ///< An error counter to pause the thread if the realm is unreachable for too long (preventing spin-lock)
		//It's a singleton
		RealmConnector();
		~RealmConnector();
};

typedef ACE_Singleton<RealmConnector,ACE_Null_Mutex> REALMCONNECTOR;

#endif