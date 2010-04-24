#include "MainClient.h"


CMainClient::CMainClient(void) :
	m_state(State_Loading), m_mainsocket(), m_networkthread(),
	m_networkthread_mutex(PTHREAD_MUTEX_INITIALIZER),
	m_networkthread_cond(PTHREAD_COND_INITIALIZER)
{
	pthread_mutex_init(&m_networkthread_mutex, NULL);
	pthread_cond_init(&m_networkthread_cond, NULL);
}

CMainClient::~CMainClient(void)
{
	pthread_mutex_destroy(&m_networkthread_mutex);
	pthread_cond_destroy(&m_networkthread_cond);
}

int CMainClient::Run(void)
{
	//==============
	//Load the GUI and display the loading screen
	//==============
	m_gui.StartThread();

	//==============
	//Load the data files
	//==============
	//datafileclass.load();

	//==============
	//Load the network system
	//==============
	if(!m_mainsocket.socket_create()) std::cout << "[ERROR] Could not create socket!\n";

	//==============
	//Connect to the realm server to retrieve the update list and world server list
	//==============
	m_state = State_LoggingInRealm;
	//Start the network thread and
	//wait for the it to complete the realm server stuff
	StartNetworkThread();
	pthread_mutex_lock(&m_networkthread_mutex);
	pthread_cond_wait(&m_networkthread_cond, &m_networkthread_mutex);
	pthread_mutex_unlock(&m_networkthread_mutex);

	//==============
	//Signal 'Done Loading' to GUI and so on
	//==============
	m_state = State_LoginScreen;
	m_gui.SendNotify();

	while( m_state != State_Quitting ){
		sleep(50);
		int GuiAction = m_gui.PeekForMessage();
		switch( GuiAction ){
			case m_gui.Message_Quit:
				m_state = State_Quitting;
				//TODO: Show unloading screen
				m_mainsocket.socket_close(); //This should make any receiver thread that is busy receiving quit
				//datafileclass.unload();
				break;
			case m_gui.Message_Login:
				if( m_state != State_LoginScreen ) break;
				m_state = State_LoggingIn;
				StartNetworkThread();
				break;
			case m_gui.Message_CharSelect:
				if( m_state != State_CharSelectScreen ) break;
				m_state = State_SelectingChar;
				StartNetworkThread();
				break;
			case m_gui.Message_NoMessages:
			default:
				break;
		}
		//TODO: Do other actions that need to be peformed every now and then.
	}

	return 0;
}

int	CMainClient::StartNetworkThread(void)
{
	//Wait for the thread to terminate if it was still running
	pthread_join(m_networkthread, NULL);
	//Create the new thread
	if( pthread_create(&m_networkthread, NULL, NetworkThreadStarter, this) ){
		std::cout << "[ERROR] pthread: pthread_create() failed!" << std::endl;
		return 0;
	}
	return 1;
}

void* NetworkThreadStarter(void* class_pointer){
	return ((CMainClient*)class_pointer)->NetworkThread();
}

void* CMainClient::NetworkThread(void)
{
	switch( m_state ){
		case State_LoggingInRealm:
			if( m_mainsocket.isConnected() ) m_mainsocket.socket_close();
			if(!m_mainsocket.socket_connect("127.0.0.1", 6131)){
				std::cout << "[ERROR] Could not connect to server!\n";
			}else{
				//This will keep looping untill an error occurred or the socket got disconnected
				#include "packetHandler.hpp"
			}
			pthread_cond_signal(&m_networkthread_cond); //Signal the main thread that this thread is done
			break;
		default:
			break;
	}
	return 0;
}
