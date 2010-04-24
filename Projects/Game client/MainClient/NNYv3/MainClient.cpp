#include "MainClient.h"

//	This somehow did not work.
//	So I just made a specific function for every thread function
//
//template <class CLASS, int CLASS::FUNC(void)>
//void* ClassThreadStarter(void* class_pointer){
//	return ((CLASS*)class_pointer)->FUNC();
//}

void* NetworkThreadStarter(void* class_pointer){
	return ((CMainClient*)class_pointer)->NetworkThread();
}



CMainClient::CMainClient(void) :
	m_state(State_Loading), m_mainsocket(), m_networkthread(),
	m_networkthread_cond(PTHREAD_COND_INITIALIZER), m_networkthread_mutex(PTHREAD_MUTEX_INITIALIZER)
{
}

CMainClient::~CMainClient(void)
{
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
	if( m_mainsocket.socket_create() == false ) std::cout << "[ERROR] Could not create socket!\n";
	pthread_create(&m_networkthread, NULL, NetworkThreadStarter, this);
	pthread_detach(m_networkthread);
	//TODO: check for errors creating the thread

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
				//Signal all threads
				pthread_cond_signal(&m_networkthread_cond);
				//datafileclass.unload();
				break;
			case m_gui.Message_Login:
				if( m_state != State_LoginScreen ) break;
				m_state = State_LoggingIn;
				pthread_cond_signal(&m_networkthread_cond);
				break;
			case m_gui.Message_NoMessages:
			default:
				break;
		}
		//TODO: Do other actions that need to be peformed every now and then.
	}

	return 0;
}

void* CMainClient::NetworkThread(void)
{
	while( m_state != State_Quitting ){
		//Wait for the start signal from the other thread
		//pthread_mutex_lock(&m_networkthread_mutex); //Only one thread using this mutex so no need to lock
		pthread_cond_wait(&m_networkthread_cond, &m_networkthread_mutex);
		//pthread_mutex_unlock(&m_networkthread_mutex);

		switch( m_state ){
			case State_LoggingIn:
				if( m_mainsocket.isConnected() ) m_mainsocket.socket_close();
				if( m_mainsocket.socket_connect("127.0.0.1", 6131) == false ){
					std::cout << "[ERROR] Could not connect to server!\n";
					break;
				}else{
					ByteArray data;
					m_mainsocket >> data;
					//parse this
				}
				break;
			default:
				break;
		}
	}
	return 0;
}
