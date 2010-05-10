#pragma once

#include "UIMain.h"
#include "ThreadMessages.h"

#include <list>
#include <vector>
#include "ZSocket.h"
#include "pthread.h"

#ifdef WIN32
    #include <winbase.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
	#define sleep(x) usleep(x)
#endif

class CMainClient : public CThreadMessages
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
		State_LoginScreen,
		State_LoggingIn,
		State_ServerSelectScreen,
		State_SelectingServer,
		State_CharSelectScreen,
		State_SelectingChar,
		State_LoadingIngame,
		State_Playing}
				m_state;

	//============
	// Network related
	//============
	ZSocket		m_mainsocket;
	pthread_t	m_networkthread;
	int			NetworkThreadRunning; //If the network thread is currently running. No true/false but instead increment/decrement when a thread starts/stops to prevent overlap.
	int			StartNetworkThread(void); //Call this from the main thread
	void*		NetworkThread(void); //The network thread
	friend		void* NetworkThreadStarter(void* class_pointer); //Helper function to give the created thread the right class pointer
	pthread_mutex_t	m_networkthread_mutex;
	pthread_cond_t	m_networkthread_cond;

	//The main packet handler
	void HandlePackets(void);
	//The capsule handlers.
	//They return 1 if they processed the packet
	//They return 0 if they did not process the packet (so try another handler)
	//They return -1 if they closed the connection
	int HandleRealm(WORD Cmd, ByteArray& capsule);
	int HandleWorldLogin(WORD Cmd, ByteArray& capsule);
	int HandleDefault(WORD Cmd, ByteArray& capsule);

	//List of ips of realm servers
	std::vector<std::string> m_RealmServers;
	//The version of this client
	DWORD		m_Revision;
	//The world server info
	std::string	m_WorldIP;
	WORD		m_WorldPort;
	//Login info
	std::string m_Username;
	std::string m_Password;

	//============
	// Gui related
	//============
	CUIMain		m_gui;
};
