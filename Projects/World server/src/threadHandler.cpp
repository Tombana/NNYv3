#include "threadHandler.h"

extern Grid     g_grid;
extern Database g_database;
extern pthread_mutex_t g_onlineList_mutex;
extern std::list<s_thread_data*> g_onlineList;

void threadHandler (SOCKET &m_socketID) {
    //====================================
    //       LITTLE CONSOLE PRINT
    //====================================
    std::cerr << "[threadHandler] Is now handling this client" << std::endl;

    //====================================
    // INITIALIZING THREAD VARS & SOCKET
    //====================================
    s_thread_data td;
    td.socket <= m_socketID; //push our socketID to the socket object :)
    td.authenticated = false;
    td.logged = false;
    td.thread = pthread_self();

    Grid::Handle grid_hdl = g_grid.createHandle(td);

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
    td.socket << packetToSend;
    //Cleaning buffer
    packetToSend.clear();

    //====================================
    // HANDLING PACKETS & EVERYTHING HERE
    //====================================
    #include "packetHandler.hpp"

    //====================================
    //    SOCKET CLOSE & CONSOLE PRINT
    //====================================
    td.socket.socket_close();

    //====================================
    //  EVEN MORE TASK BEFORE CLEANING
    //====================================
    if (td.authenticated) {
        //If you were authentificated, decrement the variable
        std::string request = "UPDATE accounts SET nbr_online=nbr_online-1 WHERE id=" + intToStr(td.accountID);
        g_database.query(request);
        //Oh and is the character logged in?!
        if (td.logged) {
            std::cerr << "Character " << td.name << " has logged out!" << std::endl;
            std::string request = "UPDATE characters SET online=0 WHERE id=" + intToStr(td.id);
            g_database.query(request);
            //Unsubcribe to Grid where your character is
            unsigned short grid_x = g_grid.getGridFromCoord(td.x);
            unsigned short grid_y = g_grid.getGridFromCoord(td.y);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x-1, grid_y-1);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x, grid_y-1);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x+1, grid_y-1);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x-1, grid_y);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x, grid_y);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x+1, grid_y);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x-1, grid_y+1);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x, grid_y+1);
            g_grid.unsubscribe(grid_hdl, td.map, grid_x+1, grid_y+1);
        }
    }

    std::cerr << "[threadHandler] Is no more handling this client!" << std::endl;

    //NO NEED PTHREAD_EXIT OR ANY RETURN!
}
