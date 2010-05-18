//=====================================
// PCKT_C_GETCHARLIST
//=====================================
case PCKT_C_GETCHARLIST:
{
    //-------- DISPLAY DEBUG INFO
    std::cerr << "[capsuleHandler] -- PCKT_C_GETCHARLIST --" << std::endl;

    if (td.authenticated) {
        //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
        std::string request = "SELECT slot,name,level,gender,online FROM characters WHERE account_id=" + intToStr(td.accountID);
        DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
        DB_ROW      rows;

        //-------- INITIALIZE NEEDED VARS FOR PROCESSING
        ByteArray   packet;

        //-------- FETCHING ENTRY IF ANY
        while (rows = g_database.fetch_row(results)) {
            packet.add<CMD>(PCKT_W_CHARLIST_ADD);
            packet.add<BYTE>((BYTE)atoi(rows[0])); //slot
            packet.addString(rows[1]); //name
            packet.add<BYTE>((BYTE)atoi(rows[2])); //level
            packet.addBool(static_cast<bool>(atoi(rows[3]))); //gender
            packet.addBool(static_cast<bool>(atoi(rows[4]))); //online
        }

        //-------- WE ARE DONE WITH THE SQL PART
        g_database.queryDone();

        //-------- SEND A REPLY (ACK) TO THE CLIENT
        packet.add<CMD>(PCKT_W_CHARLIST_EOF);
        td.socket << packet;
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

    if (td.authenticated) {
        std::string query = "DELETE FROM characters WHERE slot=" + intToStr(slotID) + " AND account_id=" + intToStr(td.accountID);
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
    bool kick   = capsule.readBool();
    std::cout << "[capsuleHandler] SlotID: " << (unsigned int)capsule.read<BYTE>() << std::endl;
    std::cout << "[capsuleHandler] Kick ghost: " << (unsigned int)kick << std::endl;

    if (td.authenticated) {
        std::cerr << "Request to enter world with character on slotID " << (unsigned int)slotID << std::endl;
        //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
        std::string request = "SELECT online,id,name,level,gender,x,y,z,map FROM characters WHERE slot=" + intToStr((unsigned int)slotID) + " AND account_id=" + intToStr(td.accountID);
        DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
        DB_ROW      rows;

        //-------- INITIALIZE NEEDED VARS FOR PROCESSING
        bool         db_online; //not defined because it won't be checked if db_false == false
        bool         db_found = false; //defined because it's checked right below

        //-------- FETCHING ENTRY IF ANY
        if (rows = g_database.fetch_row(results)) {
            db_online = static_cast<bool>(atoi(rows[0])); //online
            db_found  = true;

            td.id     = atoi(rows[1]); //id
            td.name   = rows[2]; //name
            td.level  = (BYTE)atoi(rows[3]); //level
            td.gender = static_cast<bool>(atoi(rows[4])); //gender
            td.x      = atoi(rows[5]); //x
            td.y      = atoi(rows[6]); //y
            td.z      = atoi(rows[7]); //z
            td.map    = atoi(rows[8]); //map
        }

        //-------- WE ARE DONE WITH THE SQL PART
        g_database.queryDone();

        //-------- PACKET ACK REPLY HEADING
        ByteArray    packet;

        //-------- PACKET ACK REPLY
        packet.add<CMD>(PCKT_W_ENTER_WORLD_ACK);
        if (db_found) {
            //Kick ghost character if bool enabled
            if (kick) {
                pthread_mutex_lock(&g_onlineList_mutex);
                for (std::list<s_thread_data*>::iterator it=g_onlineList.begin(); it!=g_onlineList.end(); ++it) {
                    if ((*it)->logged && (*it)->id == td.id) {
                        (*it)->socket.socket_close(); //Force disconnection
                        pthread_join((*it)->thread, NULL); //wait for the thread to be terminated
                    }
                }
                pthread_mutex_unlock(&g_onlineList_mutex);
                db_online = false; //little hack here, we know the character isn't online anymore
            }

            if (db_online) {
                packet.add<ACK>(ACK_ALREADY); //character already inGame!
            } else {
                //Here everything looks good! Let the char log in
                    packet.add<ACK>(ACK_SUCCESS);
                //Also mark the character as logged in
                    td.logged = true;
                //And perform other needed taks that i'm too lazy to take care of now like:
                    //TODO (NitriX#): [Later]Send to messenger list that you are now online
                    //TODO (NitriX#): [Later]If in any party, perform tasks it must do
                    //TODO (NitriX#): [Later]Same with guild stuff
                    //TODO (NitriX#): [Later]Everything else that must be performed at logon
                //And now the ingame stuff:
                    //Register to Grid where your character is
                        unsigned short grid_x = g_grid.getGridFromCoord(td.x);
                        unsigned short grid_y = g_grid.getGridFromCoord(td.y);
                        //first row
                        g_grid.subscribe(grid_hdl, td.map, grid_x-1, grid_y-1);
                        g_grid.subscribe(grid_hdl, td.map, grid_x, grid_y-1);
                        g_grid.subscribe(grid_hdl, td.map, grid_x+1, grid_y-1);
                        //middle row
                        g_grid.subscribe(grid_hdl, td.map, grid_x-1, grid_y);
                        grid_hdl = g_grid.subscribe(grid_hdl, td.map, grid_x, grid_y);
                        g_grid.subscribe(grid_hdl, td.map, grid_x+1, grid_y);
                        //bottom row
                        g_grid.subscribe(grid_hdl, td.map, grid_x-1, grid_y+1);
                        g_grid.subscribe(grid_hdl, td.map, grid_x, grid_y+1);
                        g_grid.subscribe(grid_hdl, td.map, grid_x+1, grid_y+1);
                    //TODO: Send a spawn packet to your grid so other players see you
                        ByteArray spawnPacket;
                        spawnPacket.add<CMD>(PCKT_X_DEBUG);
                        spawnPacket.addString("FUTURE_SPAWN_PACKET; New character is "+td.name);
                        g_grid.send(grid_hdl, spawnPacket);
                    //TODO: Also retrieve all entities around you and send them to the client
                //And the database ;o
                    std::string query = "UPDATE characters SET online=1 WHERE id=" + intToStr(td.id);
                    g_database.query(query);
                //And a debugging text x]
                std::cerr << "Character " << td.name << " has logged in!" << std::endl;
            }
        } else {
            packet.add<ACK>(ACK_NOT_FOUND); //character not found in database!
        }

        //-------- Send packet to client
        td.socket << packet;
    } else {
        std::cerr << "Packet ignored; user isn't authenticated." << std::endl;
    }
}
break;
