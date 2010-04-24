//=====================================
// PCKT_R_WELCOME
//=====================================
case PCKT_R_WELCOME:
	{
	std::cout << "[capsuleHandler] Welcome from realm server" << std::endl;
	//Send version packet
	ByteArray packetToSend;
	packetToSend.addCmd(PCKT_C_REVISION);
	packetToSend.addDword(0);
	m_mainsocket << packetToSend;
	}
    break;
//=====================================
// PCKT_R_DOWNLOAD
//=====================================
case PCKT_R_DOWNLOAD:
	{
	DWORD Revision = capsule.read<DWORD>();
	std::string URL(capsule.read<std::string>());
	std::string Path(capsule.read<std::string>());
	std::cout << "[capsuleHandler] We need to download a file. Revision: " << Revision << std::endl;
	std::cout << "[capsuleHandler] URL: " << URL.c_str() << std::endl;
	std::cout << "[capsuleHandler] Path: " << Path.c_str() << std::endl;
	}
	break;
//=====================================
// PCKT_R_DOWNLOAD_EOF
//=====================================
case PCKT_R_DOWNLOAD_EOF:
	std::cout << "[capsuleHandler] All files are up to date.\n";
	break;
//=====================================
// PCKT_R_CONNECT
//=====================================
case PCKT_R_CONNECT:
	{
	std::string IP(capsule.readString());
	WORD Port = capsule.read<WORD>();
	std::cout << "[capsuleHandler] We can connect to " << IP.c_str() << ":" << Port << std::endl;
	m_state = State_LoginScreen;
	}
	break;
//=====================================
// PCKT_R_SERVER_GONE
//=====================================
case PCKT_R_SERVER_GONE:
	std::cout << "[capsuleHandler] There are currently no world servers online!\n";
	m_state = State_LoginScreen;
	break;