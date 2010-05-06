//=====================================
// PCKT_C_AUTH
//=====================================
case PCKT_C_AUTH:
{
    //-------- DISPLAY DEBUG INFO
    std::cout << "[capsuleHandler] -- PCKT_C_AUTH --" << std::endl;
    std::string username = alphaNumOnly(capsule.readString());
    std::string password = capsule.readString();
    std::cout << "[capsuleHandler] Username: " << username << std::endl;
    std::cout << "[capsuleHandler] Password: " << password << std::endl;

    //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
    std::string request = "SELECT id,password,online FROM accounts WHERE username='" + username + "'";
    DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
    DB_ROW      rows;

    //-------- INITIALIZE NEEDED VARS FOR PROCESSING
    bool         flag_entryFound = false;
    unsigned int bd_id;
    std::string  bd_password;
    bool         bd_online;
    ByteArray    answer;

    //-------- FETCHING ENTRY IF ANY
    if (rows = g_database.fetch_row(results)) {
        bd_id           = atoi(rows[0]);
        bd_password     = rows[1];
        bd_online       = static_cast<bool>(atoi(rows[2]));
        flag_entryFound = true;
    }

    //-------- WE ARE DONE WITH THE SQL PART
    g_database.queryDone();

    //-------- CHECK THE ENTRY WE FOUND
    if (flag_entryFound) {
        std::cerr << "FOUND!" << std::endl;
    } else {
        std::cerr << "DOESN'T EXIST!" << std::endl;
        //ByteArray
    }
}
break;
