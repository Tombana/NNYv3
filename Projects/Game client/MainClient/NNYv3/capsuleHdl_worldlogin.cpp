#include "MainClient.h"
#include "protocol.hpp"

int CMainClient::HandleWorldLogin(WORD Cmd, ByteArray& capsule)
{
	switch(Cmd){
		//=====================================
		// PCKT_W_WELCOME
		//=====================================
		case PCKT_W_WELCOME:
			{
			std::cout << "[capsuleHandler] Welcome from world server" << std::endl;
			//It might be better to have a seperate function called SendLoginPacket
			//and have it called from the CMainClient message loop instead of the networkthread
			//sending it as response to the welcome packet.
			ByteArray LoginPacket;
			LoginPacket.addCmd(PCKT_C_AUTH);
			LoginPacket.addString(m_Username);
			LoginPacket.addString(m_Password);
			m_mainsocket << LoginPacket;
			}
			break;
		//=====================================
		// PCKT_W_AUTH_ACK
		//=====================================
		case PCKT_W_AUTH_ACK:
			{
				ACK code = capsule.readAck();
				bool LoggedIn = false;
				std::cout << "[capsuleHandler] ";
				switch(code){
					case ACK_SUCCESS:
						std::cout << "You are logged in!\n";
						LoggedIn = true;
						break;
					case ACK_NOT_FOUND:
						std::cout << "The server was unable to find your username in the database.\n";
						break;
					case ACK_DOESNT_MATCH:
						std::cout << "Invalid password.\n";
						break;
					case ACK_ALREADY:
						std::cout << "You were already logged in.\n";
						break;
					case ACK_REFUSED:
						std::cout << "The server explicitly refused your connection. You might be banned.\n";
						break;
					default:
						std::cout << "Error: invalid code in PCKT_W_AUTH_ACK.\n";
						break;
				}
				if( !LoggedIn ){
					m_state = State_LoginScreen;
					return -1;
				}
			}
			break;
		default:
			return 0;
	}
	return 1;
}