#include "MainClient.h"
#include <iostream>

CMainClient* CMainClient::mSingleton = 0;

CMainClient::CMainClient(void) :
	m_state(State_Loading), m_mainsocket(), m_networkthread(),
	m_networkthread_mutex(PTHREAD_MUTEX_INITIALIZER),
	m_networkthread_cond(PTHREAD_COND_INITIALIZER),
	m_Revision(0), m_WorldIP(), m_WorldPort(0),
	m_gui(), m_MessageQueue(), m_message_mutex(PTHREAD_MUTEX_INITIALIZER)
{
	pthread_mutex_init(&m_networkthread_mutex, NULL);
	pthread_cond_init(&m_networkthread_cond, NULL);
	pthread_mutex_init(&m_message_mutex, NULL);
	if( mSingleton == 0 ) mSingleton = this;
}

CMainClient::~CMainClient(void)
{
	pthread_mutex_destroy(&m_networkthread_mutex);
	pthread_cond_destroy(&m_networkthread_cond);
	if( mSingleton == this ) mSingleton = 0;
}

int CMainClient::Run(void)
{
	//==============
	//Load the UI and display the loading screen
	//==============
	if( !m_gui.StartUI() ) return 0;

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
	m_gui.SendNotify(m_gui.Message_DisplayLogin);

	while( m_state != State_Quitting ){
		sleep(50);
		//When the gui wants to notify this thread the message will get into the list.
		//Examples: the user pressed close
		//
		if( m_MessageQueue.size() ){ //If there are messages from other threads to process
			pthread_mutex_lock(&m_message_mutex);
			int GuiAction = m_MessageQueue.front(); //Get the message from the queue
			m_MessageQueue.pop_front(); //Remove the message from the queue
			pthread_mutex_unlock(&m_message_mutex);
			switch( GuiAction ){
				case Message_Quit:
					m_state = State_Quitting;
					//TODO: Show unloading screen
					m_mainsocket.socket_close(); //This should make any receiver thread that is busy receiving quit
					break;
				case Message_Login:
					if( m_state != State_LoginScreen ) break;
					m_state = State_LoggingIn;
					StartNetworkThread();
					break;
				case Message_CharSelect:
					if( m_state != State_CharSelectScreen ) break;
					m_state = State_SelectingChar;
					StartNetworkThread();
					break;
				default:
					break;
			}
		}
		//TODO: Do other actions that need to be peformed every now and then.
	}

	//datafileclass.unload();

	m_gui.SendNotify(m_gui.Message_Quit);
	m_gui.WaitForExit();
	return 0;
}

int CMainClient::SendNotify(int MessageID)
{
	if( MessageID ){
		pthread_mutex_lock(&m_message_mutex);
		m_MessageQueue.push_back(MessageID);
		pthread_mutex_unlock(&m_message_mutex);
	}
	return 1;
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
	//Disconnect the socket if it was still connected
	if( m_mainsocket.isConnected() ) m_mainsocket.socket_close();
	//Choose what we have to connect to
	switch( m_state ){
		case State_LoggingInRealm:
			if(!m_mainsocket.socket_connect("127.0.0.1", 6131)){
				std::cout << "[ERROR] Could not connect to server!\n";
			}
			break;
		case State_LoggingIn:
			if( m_WorldIP.empty() || m_WorldPort == 0 ){
				std::cout << "[ERROR] No world server specified!\n";
			}else{
				if(!m_mainsocket.socket_connect(m_WorldIP, m_WorldPort)){
					std::cout << "[ERROR] Could not connect to world server!\n";
				}
			}
			if( !m_mainsocket.isConnected() ) m_state = State_LoginScreen;
			break;
		default:
			break;
	}
	//Start the loop that receives all packets
	if( m_mainsocket.isConnected() ){
		//This will keep looping untill an error occurred or the socket got disconnected
		#include "packetHandler.hpp"
	}
	//Disconnected. Signal the main thread
	pthread_cond_signal(&m_networkthread_cond); //Signal the main thread that this thread is done
	return 0;
}
