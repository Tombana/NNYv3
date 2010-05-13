#include "MainClient.h"
#include <iostream>

CMainClient* CMainClient::mSingleton = 0;

CMainClient::CMainClient(void) :
	m_state(State_Loading), m_mainsocket(), m_networkthread(), NetworkThreadRunning(0),
	m_networkthread_mutex(PTHREAD_MUTEX_INITIALIZER),
	m_networkthread_cond(PTHREAD_COND_INITIALIZER),
	m_RealmServers(),m_Revision(0), m_WorldIP(), m_WorldPort(0),
	m_ui(),
	m_Username(), m_Password(), m_Characters()
{
	m_RealmServers.push_back("127.0.0.1");
	m_RealmServers.push_back("ceres.dlinkddns.com");
	pthread_mutex_init(&m_networkthread_mutex, NULL);
	pthread_cond_init(&m_networkthread_cond, NULL);
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
	if( !m_ui.StartUI() ) return 0;

	//==============
	//Load the data files
	//==============
	//datafileclass.load();
	
	m_Username = "remembered-username";

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

	if( m_WorldIP.empty() || !m_WorldPort )
		m_ui.SendThreadMessage(new CMessageMsgBox("Could not get world server info from realm server!", "Error"));

	//==============
	//Signal 'Done Loading' to GUI and so on
	//==============
	m_state = State_LoginScreen;
	m_ui.SendThreadMessage(new CMessageDisplayLoginScreen(m_Username));

	while( m_state != State_Quitting ){
		sleep(50);
		CMessage* msg = GetThreadMessage();
		if( msg ){ //If there are messages from other threads to process
			switch(msg->ID){
				case Message_Quit:
					m_state = State_Quitting;
					//TODO: Show unloading screen
					m_mainsocket.socket_close(); //This should make any receiver thread that is busy receiving quit
					break;
				case Message_Login:
					{
						CMessageLogin* loginmsg = (CMessageLogin*)msg;
						if( m_state != State_LoginScreen ) break;
						m_Username = loginmsg->Username;
						m_Password = loginmsg->Password;
						m_state = State_LoggingIn;
						m_ui.SendThreadMessage(new CMessageDisplayWaitScreen("Logging in..."));
						StartNetworkThread();
					}
					break;
				case Message_KickAccount:
					{
						CMessageKickAccount* kickmsg = (CMessageKickAccount*)msg;
						if( kickmsg->DoKick == false ){
							m_state = State_LoginScreen;
							m_mainsocket.socket_close();
						}else{
							ByteArray Packet;
							//Send kick packet
							Packet.addCmd(PCKT_C_KICK_GHOST_ACCOUNT);
							//Send get character list packet
							m_Characters.clear();
							Packet.addCmd(PCKT_C_GETCHARLIST);
							m_mainsocket << Packet;
						}
					}
					break;
				case Message_CharSelect:
					if( m_state != State_CharSelectScreen ) break;
					m_state = State_SelectingChar;
					StartNetworkThread();
					break;
				default:
					break;
			}
			delete msg;
		}
		//TODO: Do other actions that need to be peformed every now and then.
	}

	//datafileclass.unload();

	m_ui.SendThreadMessage(Message_Quit);
	m_ui.WaitForExit();
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
	++NetworkThreadRunning;
	//Disconnect the socket if it was still connected
	if( m_mainsocket.isConnected() ) m_mainsocket.socket_close();
	//Choose what we have to connect to
	switch( m_state ){
		case State_LoggingInRealm:
			for( std::vector<std::string>::iterator server = m_RealmServers.begin(); server != m_RealmServers.end(); ++server ){
				if(m_mainsocket.socket_connect(*server, 6131)){
					break;
				}
			}
			if( !m_mainsocket.isConnected() ) std::cout << "[ERROR] Could not connect to a realm server.\n";
			break;
		case State_LoggingIn:
			if( m_WorldIP.empty() || !m_WorldPort || !m_mainsocket.socket_connect(m_WorldIP, m_WorldPort)){
				m_state = State_LoginScreen;
				m_ui.SendThreadMessage(new CMessageMsgBox("Could not connect to the world server!", "Error"));
				m_ui.SendThreadMessage(Message_CloseWaitScreen);
			}
			break;
		default:
			break;
	}
	//Start the loop that receives all packets
	if( m_mainsocket.isConnected() ){
		//This will keep looping untill an error occurred or the socket got disconnected
		HandlePackets();
	}

	--NetworkThreadRunning;
	//Disconnected. Signal the main thread
	pthread_cond_signal(&m_networkthread_cond); //Signal the main thread that this thread is done
	return 0;
}
