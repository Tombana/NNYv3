#ifndef H_ZSOCKET
#define H_ZSOCKET

#include <iostream>
#include <string.h>
#include "ByteArray.h"
#include "config.hpp"

#if defined(WIN32)
    #include <winsock2.h>
    #include <windows.h>
    #define socklen_t int
#else
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <resolv.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #define SOCKET int
    #define SOCKADDR_IN    struct sockaddr_in
    #define SOCKADDR       struct sockaddr
    #define INVALID_SOCKET -1
    #define closesocket(s) close(s);
#endif

#define ZSOCKET_VERSION "1.3.4"

#if defined(WIN32)
    void ZSocket_loadWinsock();
    void ZSocket_unloadWinsock();
#endif

class ZSocket {
    typedef struct {
        SOCKET id; // socket id
        SOCKADDR_IN server; // server infos
    } s_socket;

public:
                ZSocket();
                ~ZSocket();
    void        operator<=(SOCKET &socketID);
    bool        socket_create();
    SOCKET      getSocketID();
    bool        socket_connect(std::string host, int port);
    bool        socket_connect(const char *host, int port);
    void        operator<<(const char *data);
    void        operator<<(std::string output);
    void        operator<<(ByteArray::ByteArray &pckt);
    void        operator>>(ByteArray &out);
    bool        socket_bind(int port);
    SOCKET      socket_acceptClient();
    bool        isConnected();
    bool        isDataReceived();
    bool        checkIsConnected();
    void        socket_close();

private:
    int         m_port; //port number
    std::string m_host; //hostname or ip
    s_socket    m_socket; //socket info
    bool        m_created; //Works!
    bool        m_connected; //Not quite sure yet
    bool        m_dataReceived;
};

#endif
