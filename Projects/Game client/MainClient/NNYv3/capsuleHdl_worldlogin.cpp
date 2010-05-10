#include "MainClient.h"
#include "protocol.hpp"

int CMainClient::HandleWorldLogin(WORD Cmd, ByteArray& capsule)
{
	int ret = 1;
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
				if( code == ACK_SUCCESS ){
					//Logged in, send the request for the character list
					m_Characters.clear();
					ByteArray CharPacket;
					CharPacket.addCmd(PCKT_C_GETCHARLIST);
					m_mainsocket << CharPacket;
				}else{
					std::string Message;
					switch(code){
						case ACK_NOT_FOUND:
							Message = "The server was unable to find your username in the database.";
							break;
						case ACK_DOESNT_MATCH:
							Message = "Invalid password.";
							break;
						case ACK_ALREADY:
							Message = "You were already logged in.";
							break;
						case ACK_REFUSED:
							Message = "The server explicitly refused your connection. You might be banned.";
							break;
						default:
							Message = "Error: invalid code in PCKT_W_AUTH_ACK.";
							break;
					}
					ret = -1;
					m_state = State_LoginScreen;
					m_ui.SendThreadMessage(new CMessageMsgBox(Message, "Error"));
					m_ui.SendThreadMessage(Message_CloseWaitScreen);
				}
			}
			break;
		//=====================================
		// PCKT_W_CHARLIST_ADD
		//=====================================
		case PCKT_W_CHARLIST_ADD:
			{
				CharacterInfo Char;
				Char.Slot = capsule.read<BYTE>();
				Char.Name = capsule.readString();
				Char.Level = capsule.read<BYTE>();
				Char.Gender = capsule.readBool();
				Char.Online = capsule.readBool();
				m_Characters.push_back(Char);
			}
			break;
		//=====================================
		// PCKT_W_CHARLIST_EOF
		//=====================================
		case PCKT_W_CHARLIST_EOF:
			{
				m_state = State_CharSelectScreen;
				m_ui.SendThreadMessage(new CMessageDisplayCharSelect(m_Characters));
				m_ui.SendThreadMessage(Message_CloseWaitScreen);
			}
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}