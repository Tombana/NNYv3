#include "MainClient.h"
#include "resProtocol.h"

int CMainClient::HandleRealm(WORD Cmd, ByteArray& capsule)
{
	switch(Cmd){
		//=====================================
		// PCKT_R_WELCOME
		//=====================================
		case PCKT_R_WELCOME:
			{
			std::cout << "[capsuleHandler] Welcome from realm server" << std::endl;
			//Send version packet
			ByteArray packetToSend;
			packetToSend.addCmd(PCKT_C_REVISION);
			packetToSend.add<DWORD>(m_Revision);
			m_mainsocket << packetToSend;
			}
			break;
		//=====================================
		// PCKT_R_DOWNLOAD
		//=====================================
		case PCKT_R_DOWNLOAD:
			{
			DWORD Revision = capsule.read<DWORD>();
			std::string URL(capsule.readString());
			std::string Path(capsule.readString());
			std::cout << "[capsuleHandler] We need to download a file. Revision: " << Revision << std::endl;
			std::cout << "[capsuleHandler] URL: " << URL.c_str() << std::endl;
			std::cout << "[capsuleHandler] Path: " << Path.c_str() << std::endl;
			}
			break;
		//=====================================
		// PCKT_R_DOWNLOAD_EOF
		//=====================================
		case PCKT_R_DOWNLOAD_EOF:
			{
			std::cout << "[capsuleHandler] All files are up to date.\n";
			m_Worlds.clear();
			ByteArray packetToSend;
			packetToSend.addCmd(PCKT_C_GETWORLDLIST);
			m_mainsocket << packetToSend;
			}
			break;
		//=====================================
		// PCKT_R_WORLD
		//=====================================
		case PCKT_R_WORLD:
			{
			WORLDSERVER Server;
			Server.IP = capsule.readString();
			Server.Port = capsule.read<WORD>();
			Server.Name = capsule.readString();
			m_Worlds.push_back(Server);
			std::cout << "[capsuleHandler] World server: " << Server.IP.c_str() << ":" << Server.Port << " - " << Server.Name.c_str() << std::endl;
			}
			break;
		//=====================================
		// PCKT_R_WORLD_EOF
		//=====================================
		case PCKT_R_WORLD_EOF:
			m_state = State_LoginScreen;
			this->SendThreadMessage(Message_RealmLoaded);
			break;
		default:
			return 0;
	}
	return 1;
}