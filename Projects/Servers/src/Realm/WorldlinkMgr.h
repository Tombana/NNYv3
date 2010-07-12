#ifndef H_WORLDLINKMGR
#define H_WORLDLINKMGR

//Std libraries
#include <string>
//This class is a singleton
#include "ace/Singleton.h"
#include "ace/Null_Mutex.h"
//Need resource files
#include "resTypedef.h"
#include "resProtocol.h"
//We need the database to load/reload the worlds
#include "database.h"
//Our containers
#include <map>
//Generate packet
#include "Packet.h"
//Save server sockets for future usage
#include "Socket.h"

class WorldlinkMgr {
	///This make our class a singleton; read notes below if you need help using it.
	friend class ACE_Singleton<WorldlinkMgr, ACE_Null_Mutex>;
public:
	//===== Member structures
	struct s_link {
		std::string ipv4;
		PORT		port;
		std::string name;
		std::string sync_key;
		bool		online;
		BYTE		flag;
		Socket		socket;
		//TODO: nb player online
		//TODO: nb max player online
		//TODO: online player list (with usernames) for messenger lookup?
	};
	//Detect added/removed servers from database & update in memory the destination IP/port if needed
	void		loadWorldsFromDB(database::connection db);
	void		reloadWorldsFromDB(database::connection db);
	//Bunch of safe 'get' functions
	Packet&		getGeneratedPacket();
	s_link*		getLink(DWORD id);
	//WARNING! createLink() / destroyLink() & isOnline() ids arn't checked for performance reasons!
	//It means that you are supposed to know what you're doing when using this class.
	void		createLink(DWORD id);
	void		destroyLink(DWORD id);
	bool		isOnline(DWORD id);
private:
	//===== Member functions
	//contructor/destructor are privates, so the class can only be constructed with the ACE_Singleton typedef define below (WORLDLINKMGR)
	WorldlinkMgr();
	~WorldlinkMgr();
	void clearWorlds();
	void preparePacket();
	//===== Member variables
	std::map<DWORD,s_link*>		m_data;
	Packet						m_generatedPacket;
};

typedef ACE_Singleton<WorldlinkMgr, ACE_Null_Mutex> WORLDLINKMGR;

#endif