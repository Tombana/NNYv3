#pragma once

#include <list>
#include "UIMain.h"
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

	//When other threads (mainly GUI thread) want to notify the main thread of something
	//they call this function
	int SendNotify(int MessageID);

	//Message constants
	static const int	Message_Quit		=	100;
	static const int	Message_Previous	=	101;
	static const int	Message_Login		=	1000;
	static const int	Message_RealmSelect	=	1001;
	static const int	Message_CharSelect	=	1002;
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

	//============
	// Network related
	//============
	ZSocket		m_mainsocket;
	pthread_t	m_networkthread;
	int			StartNetworkThread(void); //Call this from the main thread
	void*		NetworkThread(void); //The network thread
	friend		void* NetworkThreadStarter(void* class_pointer); //Helper function to give the created thread the right class pointer
	pthread_mutex_t	m_networkthread_mutex;
	pthread_cond_t	m_networkthread_cond;

	DWORD		m_Revision;

	std::string	m_WorldIP;
	WORD		m_WorldPort;

	//============
	// Gui related
	//============
	CUIMain	m_gui;

	//When other threads notify this thread the notification will be put in this vector
	std::list<int>	m_MessageQueue;
	pthread_mutex_t	m_message_mutex;
};
