//=====================================
// PCKT_C_REVISION
//=====================================
case PCKT_C_REVISION:
    { //brakets here relates to "jumping over the initialization" (ISO C++ '03 6.7/3)
    DWORD clientRevision = capsule.read<DWORD>();
    std::cout << "[capsuleHandler] -- PCKT_C_REVISION --" << std::endl;
    std::cout << "[capsuleHandler] Client revision: " << clientRevision << std::endl;
    //---------------------------------
        #define db_ptr threadData.database
        std::string request = "SELECT revision,url,relative_path FROM updates WHERE revision>" + db_ptr->intToStr(clientRevision);
        DB_RESULT result = db_ptr->query(DB_USE_RESULT, request);
        DB_ROW rows;
        ByteArray packetToSend;
        while (rows = db_ptr->fetch_row(result)) {
            packetToSend.addCmd(PCKT_R_DOWNLOAD);
            packetToSend.add<DWORD>(atoi(rows[0]));
            packetToSend.addString(rows[1]);
            packetToSend.addString(rows[2]);
        }
        db_ptr->queryDone();
        packetToSend.addCmd(PCKT_R_DOWNLOAD_EOF);
        //------------
        Dispatcher::s_current_worldserver current = threadData.dispatcher->pickupCurrent();
        if (current.gone) {
            packetToSend.addCmd(PCKT_R_SERVER_GONE);
        } else {
            packetToSend.addCmd(PCKT_R_CONNECT);
            packetToSend.addString(current.ipv4); //ip
            packetToSend.add<WORD>(current.port); //port
            srand(time(NULL));
            packetToSend.add<DWORD>(rand()); //token
        }
        //------------
        socket << packetToSend;
        socket.socket_close();
    //---------------------------------
    }
    break;
