#include "MainClient.h"
#include "resProtocol.h"
#include "ThreadMessagesConstants.h"

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
			this->SendThreadMessage(Message_WorldConnected);
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
				}else if( code != ACK_ALREADY ){
					ret = -1; //Close connection
					m_state = State_LoginScreen;
				}
				this->SendThreadMessage(new CMessageParamsLoginResponse(code));
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
				this->SendThreadMessage(new CMessageParamsDisplayCharSelect(m_Characters));
				this->SendThreadMessage(Message_CloseWaitScreen);
			}
			break;
		default:
			ret = 0;
			break;
	}
	return ret;
}