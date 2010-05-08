//=====================================
// PCKT_C_GETCHARLIST
//=====================================
case PCKT_C_GETCHARLIST:
{
    //-------- DISPLAY DEBUG INFO
    std::cerr << "[capsuleHandler] -- PCKT_C_GETCHARLIST --" << std::endl;

    if (threadDataLocal.authenticated) {
        //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
        std::string request = "SELECT slot,name,level,gender,online FROM characters WHERE account_id=" + intToStr(threadDataLocal.accountID);
        DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
        DB_ROW      rows;

        //-------- INITIALIZE NEEDED VARS FOR PROCESSING
        ByteArray   packet;

        //-------- FETCHING ENTRY IF ANY
        while (rows = g_database.fetch_row(results)) {
            packet.addCmd(PCKT_W_CHARLIST_ADD);
            packet.add<BYTE>((BYTE)atoi(rows[0])); //slot
            packet.addString(rows[1]); //name
            packet.add<BYTE>((BYTE)atoi(rows[2])); //level
            packet.addBool(static_cast<bool>(atoi(rows[3]))); //gender
            packet.addBool(static_cast<bool>(atoi(rows[4]))); //online
        }

        //-------- WE ARE DONE WITH THE SQL PART
        g_database.queryDone();

        //-------- SEND A REPLY (ACK) TO THE CLIENT
        packet.addCmd(PCKT_W_CHARLIST_EOF);
        threadData.socket << packet;
    } else {
        std::cerr << "Packet ignored; user isn't authenticated." << std::endl;
    }
}
break;

//=====================================
// PCKT_C_DELETECHAR
//=====================================
case PCKT_C_DELETECHAR:
{
    //-------- DISPLAY DEBUG INFO
    std::cerr << "[capsuleHandler] -- PCKT_C_DELETECHAR --" << std::endl;
    BYTE slotID = capsule.read<BYTE>();

    if (threadDataLocal.authenticated) {
        std::string query = "DELETE FROM characters WHERE slot=" + intToStr(slotID) + " AND account_id=" + intToStr(threadDataLocal.accountID);
        g_database.query(query);
        std::cerr << "Character on slot ID " << (unsigned int)slotID << " has been deleted if any was found" << std::endl;
    } else {
        std::cerr << "Packet ignored; user isn't authenticated." << std::endl;
    }
}
break;

//=====================================
// PCKT_C_ENTER_WORLD
//=====================================
case PCKT_C_ENTER_WORLD:
{
        //-------- DISPLAY DEBUG INFO
    std::cerr << "[capsuleHandler] -- PCKT_C_ENTER_WORLD --" << std::endl;
    BYTE slotID = capsule.read<BYTE>();

    if (threadDataLocal.authenticated) {
        std::cerr << "Request to enter world with character on slotID " << (unsigned int)slotID << std::endl;
        //TODO: PCKT_W_ENTER_WORLD_ACK stuff
    } else {
        std::cerr << "Packet ignored; user isn't authenticated." << std::endl;
    }
}
break;
