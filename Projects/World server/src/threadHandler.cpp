#include "threadHandler.h"

extern MapGrid g_mapGrid;

void threadHandler (SOCKET &m_socketID) {
    //====================================
    //       LITTLE CONSOLE PRINT
    //====================================
    std::cerr << "[threadHandler] Is now handling this client" << std::endl;

    //TODO (NitriX#): We are testing mapGrid here
    //s_mapGrid_coord nano_test(21,782268);
    //g_mapGrid.insert(MapGrid::value_type(nano_test,666));
    //std::cerr << "Wohoo!: " << g_mapGrid[nano_test] << std::endl;

    //====================================
    // INITIALIZING THREAD VARS & SOCKET
    //====================================
    s_thread_data threadData;
    threadData.socket <= m_socketID; //push our socketID to the socket object :)

    //====================================
    //       SERVER WELCOME PACKET
    //====================================
    //Creating packet buffer
    ByteArray packetToSend;
    //Welcome packet
    packetToSend.addCmd(PCKT_W_WELCOME);
    //Debug packet
    packetToSend.addCmd(PCKT_X_DEBUG);
    packetToSend.addString("World server says hi :)");
    //Writing to socket
    threadData.socket << packetToSend;
    //Cleaning buffer
    packetToSend.clear();

    //====================================
    // HANDLING PACKETS & EVERYTHING HERE
    //====================================
    #include "packetHandler.hpp"

    //====================================
    //    SOCKET CLOSE & CONSOLE PRINT
    //====================================
    std::cout << "[threadHandler] Client logged out!" << std::endl;
    threadData.socket.socket_close();
    //NO NEED PTHREAD_EXIT OR ANY RETURN!
}
