//Next available PCKT code is         0x000D //PCKT Code 13

#define NNY_PROTOCOL_VERSION    28

/*****************************
        [X] Multi-directional
            Debug / Temporary
*****************************/
#define PCKT_X_DEBUG        0x0009 //PCKT Code 8
//Info: Send a string for debugging purpose
//Params: [PSTR]Text

/*****************************
        [C]lient packets
*****************************/
#define PCKT_C_REVISION     0x0002 //PCKT Code 2
//Info: Send your revision to the realm server
//Params: [DWORD]Client version
#define PCKT_C_AUTH         0x000C //PCKT Code 12
//Info: Authentication to the world server
//Params: [String]Username (will be lowercased by the server)
//Params: [String]Password (can be hashed with md5+salt or whatever)
//Params: [Int]Token

/*****************************
        [R]ealm server
*****************************/
#define PCKT_R_HACKING_ATTEMPT 0x000A //PCKT Code 10
//Info: Hacking attempt detected!
//Params: [None]
#define PCKT_R_WELCOME      0x0001 //PCKT Code 1
//Info: Server signature sent by the realm server to every sockets
//Additional: It doesnt send the server version anymore,
//Params: [None]
#define PCKT_R_DOWNLOAD     0x0003 //PCKT Code 3
//Info: Force the client to download a file from a URL and save it to a RELATIVE PATH.
//Additional: You must send PCKT_R_DOWNLOAD_EOF after all those packets.
//Params: [Dword]Is from what revision number
//Params: [String]URL with HTTP prefix ie: http://domain.com/folder/3451.dat
//Params: [String]Relative path ie: /folder/anotherfolder2/file.ext
#define PCKT_R_DOWNLOAD_EOF 0x0004 //PCKT Code 4
//Info: Tell the client he got the full list of what he must download, so now just proceed, kay thanks.
//Params: [None]
#define PCKT_R_CONNECT      0x0005 //PCKT Code 5
//Info: Tell the client to connect to the specified worldserver
//Params: [String]ipv4
//Params: [Word]port
//Params: [Dword]token
#define PCKT_R_SYNC_KEY_ACK 0x0007 //PCKT Code 7
//Info: Tell the server if its authorized or not
//Params: [Bool] True=Success / False=Failure
#define PCKT_R_SERVER_GONE  0x0008
//Info: Tell the client there's no server online
//Params: [None]

/*****************************
        [W]orld server
*****************************/
#define PCKT_W_SYNC_KEY     0x0006 //PCKT Code 6
//Info: Server signature sent to the realm server to be authorized & online
//Additional: Realmserver will reply with PCKT_R_SYNC_KEY_ACK
//Params: [dword]serverID
//Params: [pstr]sync_key

#define PCKT_W_WELCOME      0x000B //PCKT Code 11
//Info: Server signature sent by the world server to every sockets
//Params: [None]
