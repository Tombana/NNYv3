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
    std::cout << "[capsuleHandler] SlotID: " << (unsigned int)capsule.read<BYTE>() << std::endl;
    std::cout << "[capsuleHandler] Kick ghost: " << (unsigned int)capsule.readBool() << std::endl;

    //TODO (NitriX#): Kick ghost character if bool enabled

    if (threadDataLocal.authenticated) {
        std::cerr << "Request to enter world with character on slotID " << (unsigned int)slotID << std::endl;
        //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
        std::string request = "SELECT online,id,name,level,gender,x,y,z FROM characters WHERE slot=" + intToStr((unsigned int)slotID) + " AND account_id=" + intToStr(threadDataLocal.accountID);
        DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
        DB_ROW      rows;

        //-------- INITIALIZE NEEDED VARS FOR PROCESSING
        bool         db_online; //not defined because it won't be checked if db_false == false
        bool         db_found = false; //defined because it's checked right below

        //-------- FETCHING ENTRY IF ANY
        if (rows = g_database.fetch_row(results)) {
            db_online = static_cast<bool>(atoi(rows[0])); //online
            db_found  = true;

            TDL.id     = atoi(rows[1]); //id
            TDL.name   = rows[2]; //name
            TDL.level  = (BYTE)atoi(rows[3]); //level
            TDL.gender = static_cast<bool>(atoi(rows[4])); //gender
            TDL.x      = atoi(rows[5]); //x
            TDL.y      = atoi(rows[6]); //y
            TDL.z      = atoi(rows[7]); //z
        }

        //-------- WE ARE DONE WITH THE SQL PART
        g_database.queryDone();

        //-------- PACKET ACK REPLY HEADING
        ByteArray    packet;

        //-------- PACKET ACK REPLY
        packet.addCmd(PCKT_W_ENTER_WORLD_ACK);
        if (db_found) {
            if (db_online) {
                packet.addAck(ACK_ALREADY); //character already inGame!
            } else {
                //Here everything looks good! Let the char log in
                    packet.addAck(ACK_SUCCESS);
                //Also mark the character as logged in
                    TDL.logged = true;
                //And perform other needed taks that i'm too lazy to take care of now like:
                    //TODO (NitriX#): [Later]Send to messenger list that you are now online
                    //TODO (NitriX#): [Later]If in any party, perform tasks it must do
                    //TODO (NitriX#): [Later]Same with guild stuff
                    //TODO (NitriX#): [Later]Everything else that must be performed at logon
                //And now the ingame stuff:
                    //TODO: Register to Grid where your character is
                    //TODO: Send a spawn packet to your grid so other players see you
                    //TODO: Also retrieve all entities around you and send them to the client
                //And the database ;o
                    std::string query = "UPDATE characters SET online=1 WHERE id=" + intToStr(TDL.id);
                    g_database.query(query);
                //And a debugging text x]
                std::cerr << "Character " << TDL.name << " has logged in!" << std::endl;
            }
        } else {
            packet.addAck(ACK_NOT_FOUND); //character not found in database!
        }

        //-------- Send packet to client
        threadData.socket << packet;
    } else {
        std::cerr << "Packet ignored; user isn't authenticated." << std::endl;
    }
}
break;
