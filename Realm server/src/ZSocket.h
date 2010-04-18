#ifndef H_ZSOCKET
#define H_ZSOCKET

#include <iostream>
#include <string.h>
#include "ByteArray.h"

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

#define ZSOCKET_VERSION "1.2"

class ZSocket {
    typedef struct {
        SOCKET id; // socket id
        SOCKADDR_IN server; // server infos
    } s_socket;

public:
                ZSocket();
                ZSocket(SOCKET socketID);
                ~ZSocket();
    bool        socket_connect(std::string host, int port, bool persistant);
    void        socket_write(const char *data);
    void        socket_write(std::string output);
    void        socket_write(ByteArray::ByteArray &pckt);
    bool        socket_bind(int port);
    SOCKET      getSocketID();
    void        setSocketID(SOCKET socketID);
    ByteArray   socket_read(int nbBytes);
    SOCKET      socket_acceptClient();
    bool        isAlive();
    bool        checkIsAlive();
    void        socket_close();

private:
    void        _socket_init();
    bool        _socket_create();
    bool        m_connected; //TODO (NitriX#): if the connection is established, i think this doesnt works
    bool        m_alive;
    int         m_port; //port number
    std::string m_host; //hostname or ip
    s_socket    m_socket; //socket info
    #if defined(WIN32)
    bool        m_created;
    #endif
};

#endif
