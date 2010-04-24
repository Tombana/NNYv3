#pragma once

#include "GUI.h"
#include "ZSocket.h"
#include "pthread.h"

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
		State_LoginScreen,
		State_LoggingIn,
		State_RealmScreen,
		State_WaitForRealmResponse,
		State_CharSelectScreen,
		State_WaitForCharSelectResponse,
		State_LoadingIngame,
		State_Playing}
				m_state;

	//Network related
	ZSocket		m_mainsocket;
	pthread_t	m_networkthread;
	void*		NetworkThread(void);
	friend		void* NetworkThreadStarter(void* class_pointer);
	pthread_cond_t	m_networkthread_cond;
	pthread_mutex_t	m_networkthread_mutex;

	CGUI		m_gui;
};
