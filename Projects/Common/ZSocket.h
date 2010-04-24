#ifndef H_ZSOCKET
#define H_ZSOCKET

#include <iostream>
#include <string.h>
#include "pthread.h"
#include "ByteArray.h"

#if defined(WIN32)
	#include <windows.h>
    #include <winsock.h>
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

#define CONFIG_ZSOCKET_MAX_PENDING_CONNECTION       10
#define CONFIG_ZSOCKET_READ_BUFFER_SIZE             1024

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
    void        operator<<(ByteArray &pckt);
    void        operator>>(ByteArray &out);
    bool        socket_bind(int port);
    SOCKET      socket_acceptClient();
    bool        isConnected();
    bool        isDataReceived();
    bool        checkIsConnected();
    void        socket_close();

#if defined(WIN32)
protected:
    void        _loadWinsock();
    void        _unloadWinsock();
#endif

private:
    int              m_port; //port number
    std::string      m_host; //hostname or ip
    s_socket         m_socket; //socket info
    bool             m_created; //Works!
    bool             m_connected; //Not quite sure yet
    bool             m_dataReceived;
    pthread_mutex_t  m_mutex;
};

#endif
