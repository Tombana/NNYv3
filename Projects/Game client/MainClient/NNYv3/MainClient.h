#pragma once

#include "GUI.h"
#include "ZSocket.h"
#include "pthread.h"
#include "protocol.hpp"

#ifdef WIN32
    #include <windows.h>
    #define sleep(x) Sleep(x)
#else
    #include <unistd.h>
	#define sleep(x) usleep(x)
#endif

class CMainClient
{
public:
	CMainClient(void);
	~CMainClient(void);
	int Run(void);

private:
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

	//Network related
	ZSocket		m_mainsocket;
	pthread_t	m_networkthread;
	int			StartNetworkThread(void); //Call this from the main thread
	void*		NetworkThread(void); //The network thread
	friend		void* NetworkThreadStarter(void* class_pointer); //Helper function to give the created thread the right class pointer
	pthread_mutex_t	m_networkthread_mutex;
	pthread_cond_t	m_networkthread_cond;

	CGUI		m_gui;
};
