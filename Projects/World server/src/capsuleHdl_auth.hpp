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
    std::cout << "[capsuleHandler] Kick ghost: " << (unsigned int)capsule.readBool() << std::endl;

    //TODO (NitriX#): Kick ghost account if bool enabled

    //-------- PERFORM SQL QUERY & CREATE A 'RESULTS' POINTER & A 'ROWS' ARRAY
    std::string request = "SELECT id,password,nbr_online,banned FROM accounts WHERE username='" + username + "'";
    DB_RESULT  *results = g_database.query(DB_USE_RESULT, request);
    DB_ROW      rows;

    //-------- INITIALIZE NEEDED VARS FOR PROCESSING
    bool         flag_entryFound = false;
    unsigned int db_id;
    std::string  db_password;
    unsigned int db_nbr_online;
    bool         db_banned;
    ByteArray    answer;

    //-------- FETCHING ENTRY IF ANY
    if (rows = g_database.fetch_row(results)) {
        db_id           = atoi(rows[0]);
        db_password     = rows[1];
        db_nbr_online   = atoi(rows[2]);
        db_banned       = static_cast<bool>(atoi(rows[3]));
        flag_entryFound = true;
    }

    //-------- WE ARE DONE WITH THE SQL PART
    g_database.queryDone();

    //-------- CHECK THE ENTRY WE FOUND
    answer.addCmd(PCKT_W_AUTH_ACK);
    if (flag_entryFound) {
        if (db_nbr_online > 0 && !CONFIG_MULTIPLE_LOGGING_ALLOWED) {
            answer.addAck(ACK_ALREADY); //Already some characters online and dual logging is not allowed
        } else {
            if (db_password == password) {
                if (!db_banned) {
                    answer.addAck(ACK_SUCCESS); //Success!
                    request = "UPDATE accounts SET nbr_online=nbr_online+1 WHERE id=" + intToStr(db_id);
                    g_database.query(request);
                    threadDataLocal.accountID = db_id;
                    threadDataLocal.authenticated = true;
                } else {
                    answer.addAck(ACK_REFUSED); //Refused because the account is banned
                }
            } else {
                answer.addAck(ACK_DOESNT_MATCH); //Password doesn't match
            }
        }
    } else {
        answer.addAck(ACK_NOT_FOUND); //Not found
    }

    //-------- SEND A REPLY (ACK) TO THE CLIENT
    threadData.socket << answer;
}
break;
