#include "threadHandler.h"

//extern Grid     g_grid;
    //Use it like this:
    //Create an handle and store it
    //Grid::Handle grid_hdl = g_grid.createHandle(threadData);
extern Database g_database;

void threadHandler (SOCKET &m_socketID) {
    //====================================
    //       LITTLE CONSOLE PRINT
    //====================================
    std::cerr << "[threadHandler] Is now handling this client" << std::endl;

    //====================================
    // INITIALIZING THREAD VARS & SOCKET
    //====================================
    s_thread_data threadData;
    threadData.socket <= m_socketID; //push our socketID to the socket object :)

    s_thread_data_local threadDataLocal;
    threadDataLocal.authenticated = false;

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

    //====================================
    //  EVEN MORE TASK BEFORE CLEANING
    //====================================
    if (threadDataLocal.authenticated) {
        std::string request = "UPDATE accounts SET nbr_online=nbr_online-1 WHERE id=" + intToStr(threadDataLocal.accountID);
        g_database.query(request);
    }

    //NO NEED PTHREAD_EXIT OR ANY RETURN!
}
