#include "ZSocket.h"

ZSocket::ZSocket() {
    _socket_init();
    _socket_create();
}

void ZSocket::_socket_init() {
    m_connected = false; //just to make sure
    m_alive = false; //just to make sure
    #if defined(WIN32)
    m_created = false; //just to make sure
    #endif
}

//Is a socket holder mode, it doesnt create a socket actually, it only sets m_socket.id on the target object.
void ZSocket::operator<=(SOCKET &socketID) {
    _socket_init(); //set defaut bars
    m_socket.id = socketID; //set socket ID
    //ACTUALLY DONT CREATE THE SOCKET
}

ZSocket::~ZSocket() {
    //In case the guy opened a socket, we must close it when the object get destroyed, so
    if (m_created) socket_close();
}

bool ZSocket::_socket_create() {
#if defined(WIN32)
    int error=0;
    m_created = true;
#endif
    bool failed=false;

    //===================================
    //Definition Winsock [WIN32]
    //===================================
#if defined(WIN32)
    WSADATA init_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
#endif

    //===================================
    // Loading Winsock [WIN32]
    //===================================
#if defined(WIN32)
    error=WSAStartup(MAKEWORD(2,2),&init_win32);
    if (error!=0) {
        #if CONFIG_VERBOSE >= CONFIG_VERBOSE_IMPORTANT
            std::cerr << "[ZSocket] @ERROR: ZSocket failed to load Winsock: #" << error << std::endl;
        #endif
        failed=true;
    } else {
        //std::cout << "[ZSocket] Loading winsock:\tOK" << std::endl;
    }
#endif

    //===================================
    // Creating socket
    //===================================
    m_socket.id = socket(AF_INET,SOCK_STREAM,0);
    if (m_socket.id==INVALID_SOCKET) {
        #if CONFIG_VERBOSE >= CONFIG_VERBOSE_IMPORTANT
            std::cerr << "[ZSocket] @ERROR: ZSock failed to create socket" << std::endl;
        #endif
        failed=true;
    } else {
        //std::cout << "[ZSocket] Creating socket:\tOK" << std::endl;
    }

    if (failed) {
        return false;
    } else {
        return true;
    }
}

//WIP
bool ZSocket::socket_bind(int port) {
    //Descriptors
    m_socket.server.sin_family = AF_INET;
    m_socket.server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_socket.server.sin_port = htons(port);
    bool result;

    //Bind
    if (bind(m_socket.id,(struct sockaddr *)&m_socket.server,sizeof(m_socket.server)) == 0) {
        result = true; //0, succes
    } else {
        result = false; //-1, error
    }

    //Listen
    listen(m_socket.id,CONFIG_MAX_PENDING_CONNECTION); //max connections pending

    return result;
}

SOCKET ZSocket::socket_acceptClient() {
    s_socket clientInfo;
    socklen_t size = sizeof(clientInfo.server);
    SOCKET socketID = accept(m_socket.id, (struct sockaddr *)&clientInfo.server,&size);
    return socketID;
}

bool ZSocket::socket_connect(std::string host, int port) {
    return socket_connect(host.c_str(), port);
}

bool ZSocket::socket_connect(const char *host, int port) {
    int error=0;
    // Creating session
    m_socket.server.sin_family=AF_INET;
    m_socket.server.sin_addr.s_addr=inet_addr(host); // ip serveur
    m_socket.server.sin_port=htons(port); // Port écouté du serveur
    //std::cout << "[ZSocket] Connecting...\n";
    error=connect(m_socket.id,(struct sockaddr*)&m_socket.server,sizeof(m_socket.server));
    if (error!=0) {
        //std::cerr <<"[ZSocket] @ERROR: ZSocket failed to connect: #" << error << std::endl;
        m_connected = false;
    } else {
        //std::cout <<"[ZSocket] Connection established! :)" << std::endl;
        m_connected = true;
    }

    return m_connected;
}

SOCKET ZSocket::getSocketID() {
    return m_socket.id;
}

void ZSocket::operator<<(const char *data) {
    send(m_socket.id, data, strlen(data), 0);
}

void ZSocket::operator<<(ByteArray::ByteArray &pckt) {
    socket_write(pckt.genPacket());
}

void ZSocket::operator<<(std::string output) {
    send(m_socket.id, output.c_str(), output.size(), 0);
}

void ZSocket::socket_write(const char *data) {
    send(m_socket.id, data, strlen(data), 0);
}

void ZSocket::socket_write(ByteArray::ByteArray &pckt) {
    socket_write(pckt.genPacket());
}

void ZSocket::socket_write(std::string output) {
    send(m_socket.id, output.c_str(), output.size(), 0);
}

void ZSocket::socket_close() {
    shutdown(m_socket.id,2);
    closesocket(m_socket.id);
    m_connected = false;
    m_alive = false;

    #if defined(WIN32)
        if (m_created) {
            int error = 0;
            error=WSACleanup();
            if (error!=0) {
                #if CONFIG_VERBOSE >= CONFIG_VERBOSE_IMPORTANT
                    std::cerr << "[ZSocket] @ERROR: ZSocket failed to cleanup WSA out of memory: " << error << std::endl;
                #endif
                exit(-1);
            } else {
                //std::cout << "[ZSocket] WSACleanup:\t\tOK" << std::endl;
            }
        }
    #endif
}

//! Read bytes from the socket
//! @return a ByteArray object (initialized with nbBytesRead as parameter)
ByteArray ZSocket::socket_read(int nbBytes) {
    char buffer[nbBytes];
    int bytesRead;

    bytesRead = recv(m_socket.id, &buffer[0], nbBytes, 0);
    ByteArray output(buffer, bytesRead);

    if (bytesRead >= 0) {
        m_alive = true;
    } else {
        m_alive = false;
    }

    return output;
}

bool ZSocket::isAlive() {
    return m_alive;
}

bool ZSocket::checkIsAlive() {
    int bytesRead = recv(m_socket.id, NULL, 0, 0);
    if (bytesRead == 0) {
        m_alive = true;
    } else {
        m_alive = false;
    }
    return m_alive;
}
