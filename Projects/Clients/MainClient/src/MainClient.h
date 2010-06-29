#pragma once

#include <list>
#include <vector>

#include "ZSocket.h"
#include "Structures.h"
#include "UIMain.h"
#include "Thread.h"
#include "ThreadMessages.h"

#ifdef WIN32
    #include <winbase.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
	#define sleep(x) usleep(x)
#endif


class CMainClient : public CThreadMessages, public Thread
{
public:
	CMainClient(void);
	~CMainClient(void);

	static CMainClient& getSingleton(){ return *mSingleton; }
	static CMainClient* getSingletonPtr(){ return mSingleton; }

	int Run(void);

private:
	//Singleton
	static CMainClient	*mSingleton;

	//The current state of the client
	enum{ State_Loading = 0,
		State_Quitting,
		State_LoggingInRealm,
		State_WorldSelectScreen,
		State_SelectingWorld,
		State_LoginScreen,
		State_LoggingIn,
		State_CharSelectScreen,
		State_SelectingChar,
		State_LoadingIngame,
		State_Playing}
				m_state;

	//============
	// Network related
	//============
	ZSocket		m_mainsocket;
	volatile int NetworkThreadRunning; //If the network thread is currently running. No true/false but instead increment/decrement when a thread starts/stops to prevent overlap.
	int			StartNetworkThread(void); //Call this from the main thread
	void run(){ NetworkThread(); }
	void*		NetworkThread(void); //The network thread


	//The main packet handler
	void HandlePackets(void);
	//The capsule handlers.
	//They return 1 if they processed the packet
	//They return 0 if they did not process the packet (so try another handler)
	//They return -1 if they closed the connection
	int HandleRealm(WORD Cmd, ByteArray& capsule);
	int HandleWorldLogin(WORD Cmd, ByteArray& capsule);
	int HandleDefault(WORD Cmd, ByteArray& capsule);

	//List of ips of realm servers (static list, imbedded in the client)
	std::vector<std::string> m_RealmServers;
	//The version of this client
	DWORD		m_Revision;
	//The list of world servers (list gotten from realm server)
	std::vector<WORLDSERVER> m_Worlds;
	int m_WorldServer; //Index of chosen server.

	//============
	// Gui related
	//============
	CUIMain		m_ui;

	//============
	// Game related
	//============
	std::string				m_Username;
	std::string				m_Password;
	std::vector<CharacterInfo>	m_Characters;
};
