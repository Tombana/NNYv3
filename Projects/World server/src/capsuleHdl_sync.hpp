//=====================================
// PCKT_W_SYNC_KEY
//=====================================
case PCKT_W_SYNC_KEY:
    { //brakets here relates to "jumping over the initialization" (ISO C++ '03 6.7/3)
    std::cout << "[capsuleHandler] -- PCKT_W_SYNC_KEY --" << std::endl;
    DWORD serverID = capsule.read<DWORD>();
    std::string sync_key = capsule.readString();
    std::cout << "[capsuleHandler] serverID: " << serverID << std::endl;
    std::cout << "[capsuleHandler] sync_key: " << sync_key << std::endl;
    //-----------------------------------------

    //-----------------------------------------
    }
    break;
