#include "MainClient.h"
#include <iostream>
#include "resProtocol.h"
#include "ThreadMessagesConstants.h"

CMainClient* CMainClient::mSingleton = 0;

CMainClient::CMainClient(void) : CThreadMessages(), Thread(),
	m_state(State_Loading), m_mainsocket(), NetworkThreadRunning(0),
	m_RealmServers(),m_Revision(0), m_Worlds(), m_WorldServer(0),
	m_ui(),
	m_Username(), m_Password(), m_Characters()
{
	if( mSingleton == 0 ) mSingleton = this;
	m_RealmServers.push_back("127.0.0.1");
	m_RealmServers.push_back("ceres.dlinkddns.com");
}

CMainClient::~CMainClient(void)
{
	if( mSingleton == this ) mSingleton = 0;
}

int CMainClient::Run(void)
{
	m_state = State_Loading;

	//==============
	//Load the UI and display the loading screen
	//==============
	if( !m_ui.StartUI() ) return 0;

	//==============
	//Load the data files. Possible to use Ogre's virtual file system?
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
	//wait for the it to complete the realm server stuff (it will send a message when done)
	StartNetworkThread();

	while( m_state != State_Quitting ){
		sleep(50);
		MESSAGE msg = {0};
		GetThreadMessage(msg);
		bool DeleteMsg = true;
		switch(msg.ID){
			case Message_Quit:
				m_state = State_Quitting;
				//TODO: Show unloading screen
				m_mainsocket.socket_close(); //This should make any receiver thread that is busy receiving quit
				break;
			case Message_RealmLoaded:
				if( m_Worlds.empty() ) m_ui.SendThreadMessage(new CMessageParamsMsgBox("Could not get world server info from realm server!", "Error"));
				//Signal 'Done Loading' to GUI and so on
				m_state = State_LoginScreen;
				m_ui.SendThreadMessage(new CMessageParamsDisplayLoginScreen(m_Username));
				break;
			case Message_Login:
				{
					CMessageParamsLogin* loginmsg = (CMessageParamsLogin*)msg.params;
					if( m_state != State_LoginScreen ) break;
					m_Username = loginmsg->Username;
					m_Password = loginmsg->Password;
					m_state = State_LoggingIn;
					m_ui.SendThreadMessage(new CMessageParamsDisplayWaitScreen("Logging in..."));
					StartNetworkThread();
				}
				break;
			case Message_KickAccount:
				{
					CMessageParamsKickAccount* kickmsg = (CMessageParamsKickAccount*)msg.params;
					if( kickmsg->DoKick == false ){
						m_state = State_LoginScreen;
						m_ui.SendThreadMessage(new CMessageParamsDisplayLoginScreen(m_Username));
						m_mainsocket.socket_close();
					}else{
						//Log in again with kick set to true
						ByteArray LoginPacket;
						LoginPacket.addCmd(PCKT_C_AUTH);
						LoginPacket.addString(m_Username);
						LoginPacket.addString(m_Password);
						LoginPacket.addBool(true); //Wether to kick the account that is logged in.
						m_mainsocket << LoginPacket;
					}
				}
				break;
			case Message_CharSelect:
				if( m_state != State_CharSelectScreen ) break;
				m_state = State_SelectingChar;
				StartNetworkThread();
				break;
			//
			//Forwards: (Since threadmessages are 1-reader-1-writer the network thread can't send directly to gui thread)
			//
			case Message_NoWorld:
			case Message_CloseWaitScreen:
			case Message_LoginResponse:
			case Message_DisplayCharSelect:
				m_ui.SendThreadMessage(msg);
				DeleteMsg = false;
				break;
			default:
				break;
		}
		if( DeleteMsg ) delete msg.params;
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
	wait();
	//Create the new thread
	return (start() == true);
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
			if( !m_mainsocket.isConnected() ){
				this->SendThreadMessage(Message_RealmLoaded);
				std::cout << "[ERROR] Could not connect to a realm server.\n";
			}
			break;
		case State_LoggingIn:
			{
				WORLDSERVER& Server = m_Worlds[m_WorldServer];
				if( Server.IP.empty() || !Server.Port || !m_mainsocket.socket_connect(Server.IP, Server.Port)){
					m_state = State_LoginScreen;
					this->SendThreadMessage(Message_NoWorld);
				}
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
	return 0;
}
