//Next available PCKT CODE is      0x000E //PCKT decimal = 14

#define NNY_PROTOCOL_VERSION    31

/*****************************
        [X] Multi-directional
            Debug / Temporary
*****************************/
#define PCKT_X_DEBUG        0x0009 //PCKT decimal = 9
//Info: Send a string for debugging purpose
//Params: [PSTR]Text

/*****************************
        [C]lient packets
*****************************/
#define PCKT_C_REVISION     0x0002 //PCKT decimal = 2
//Info: Send your revision to the realm server
//Params: [DWORD]Client version
#define PCKT_C_AUTH         0x000C //PCKT decimal = 12
//Info: Authentication to the world server
//Params: [String]Username (will be lowercased by the server)
//Params: [String]Password (can be hashed with md5+salt or whatever)

/*****************************
        [R]ealm server
*****************************/
#define PCKT_R_HACKING_ATTEMPT 0x000A //PCKT decimal = 10
//Info: Hacking attempt detected!
//Params: [None]
#define PCKT_R_WELCOME      0x0001 //PCKT decimal = 1
//Info: Server signature sent by the realm server to every sockets
//Additional: It doesnt send the server version anymore,
//Params: [None]
#define PCKT_R_DOWNLOAD     0x0003 //PCKT decimal = 3
//Info: Force the client to download a file from a URL and save it to a RELATIVE PATH.
//Additional: You must send PCKT_R_DOWNLOAD_EOF after all those packets.
//Params: [Dword]Is from what revision number
//Params: [String]URL with HTTP prefix ie: http://domain.com/folder/3451.dat
//Params: [String]Relative path ie: /folder/anotherfolder2/file.ext
#define PCKT_R_DOWNLOAD_EOF 0x0004 //PCKT decimal = 4
//Info: Tell the client he got the full list of what he must download, so now just proceed, kay thanks.
//Params: [None]
#define PCKT_R_CONNECT      0x0005 //PCKT decimal = 5
//Info: Tell the client to connect to the specified worldserver
//Params: [String]ipv4
//Params: [Word]port
#define PCKT_R_SYNC_KEY_ACK 0x0007 //PCKT decimal = 7
//Info: Tell the server if its authorized or not
//Params: [ACK] Ack code reply
//Possible replies:
//[ACK_FAILURE] = The realm server refused the sync_key
//[ACK_SUCCESS] = The realm server accepted the sync_key and you're now online

//Params: [Bool] True=Success / False=Failure
#define PCKT_R_SERVER_GONE  0x0008 //PCKT decimal = 8
//Info: Tell the client there's no server online
//Params: [None]

/*****************************
        [W]orld server
*****************************/
#define PCKT_W_SYNC_KEY     0x0006 //PCKT decimal = 6
//Info: Server signature sent to the realm server to be authorized & online
//Additional: Realmserver will reply with PCKT_R_SYNC_KEY_ACK
//Params: [dword]serverID
//Params: [pstr]sync_key
#define PCKT_W_WELCOME      0x000B //PCKT decimal = 11
//Info: Server signature sent by the world server to every sockets
//Params: [None]
#define PCKT_W_AUTH_ACK     0x000D //PCKT decimal = 13
//Info: Reply to the authentification request PCKT_C_AUTH
//Params: [ACK] Ack code reply
//Possible replies:
//[ACK_SUCCESS] = Good, you are now logged in
//[ACK_NOT_FOUND] = We were unable to find your username in the database
//[ACK_DOESNT_MATCH] = The password provided doesn't match your username
//[ACK_ALREADY] = It seems like you are already logged in
//[ACK_REFUSED] = The server explicitly refused your connection; you maybe are banned.

/*****************************
       ACK codes (BYTEs)
  next available is number 0x06 (decimal 6)
*****************************/
#define ACK_FAILURE         0x00
#define ACK_SUCCESS         0x01
#define ACK_NOT_FOUND       0x02
#define ACK_DOESNT_MATCH    0x03
#define ACK_ALREADY         0x04
#define ACK_REFUSED         0x05
