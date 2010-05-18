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
    #define db_ptr threadData.database
    std::string request = "SELECT name FROM worldservers WHERE id=" + db_ptr->intToStr(serverID) + " AND sync_key='" + sync_key + "'";
    DB_RESULT result = db_ptr->query(DB_USE_RESULT, request);
    DB_ROW rows;
    ByteArray packetToSend;
    bool found = false;
    packetToSend.add<CMD>(PCKT_R_SYNC_KEY_ACK);
    while (rows = db_ptr->fetch_row(result)) {
        packetToSend.add<ACK>(ACK_SUCCESS);
        threadData.dispatcher->setOnline(serverID);
        threadData.is_a_worldserver = true;
        threadData.worldserverID = serverID;
        found = true;

    }
    db_ptr->queryDone();
    if (!found) packetToSend.add<ACK>(ACK_FAILURE);
    socket << packetToSend.genPacket();
    //-----------------------------------------
    }
    break;
