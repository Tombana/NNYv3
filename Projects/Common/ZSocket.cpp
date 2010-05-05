#include "ZSocket.h"

ZSocket::ZSocket() {
    m_created = false; //just to make sure
    m_connected = false; //just to make sure
    pthread_mutex_init(&m_mutex, NULL);
    //m_mutex = PTHREAD_MUTEX_INITIALIZER;
    #if defined(WIN32)
        _loadWinsock();
    #endif
}

//Is a socket holder mode, it doesnt create a socket actually, it only sets m_socket.id on the target object.
void ZSocket::operator<=(SOCKET &socketID) {
    m_socket.id = socketID; //set socket ID
}

ZSocket::~ZSocket() {
    //In case the guy opened a socket, we must close it when the object get destroyed, so
    if (m_connected || m_created) socket_close();
    #if defined(WIN32)
        _unloadWinsock();
    #endif
}

bool ZSocket::socket_create() {
    //Init vars
    m_created = true;
    bool succeeded = true;
    // Creating socket
    m_socket.id = socket(AF_INET,SOCK_STREAM,0);
    if (m_socket.id==INVALID_SOCKET) {
        std::cerr << "[ZSocket] @ERROR: ZSock failed to create socket" << std::endl;
        succeeded = false;
    }
    //Return
    return succeeded;
}

bool ZSocket::socket_bind(int port) {
    //Create a socket if not already created
    if (!m_created) socket_create();
    //Descriptors
    m_socket.server.sin_family = AF_INET;
    m_socket.server.sin_addr.s_addr = htonl(INADDR_ANY);
    m_socket.server.sin_port = htons(port);
    //Bind
    bool succeeded = false;
    if (bind(m_socket.id,(struct sockaddr *)&m_socket.server,sizeof(m_socket.server)) == 0) {
        succeeded = true; //0, succes
    }
    //Listen
    listen(m_socket.id,CONFIG_ZSOCKET_MAX_PENDING_CONNECTION); //max connections pending
    //Return
    return succeeded;
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
    //Create if socket if not yet created
    if (!m_created) socket_create();
    //Creating session
    m_socket.server.sin_family=AF_INET;
    m_socket.server.sin_addr.s_addr=inet_addr(host); //remote ip
    m_socket.server.sin_port=htons(port); //remote port
    //Connecting
    if (connect(m_socket.id,(struct sockaddr*)&m_socket.server,sizeof(m_socket.server)) == 0) {
        m_connected = true;
    } else {
        m_connected = false;
    }
    //Return var
    return m_connected;
}

SOCKET ZSocket::getSocketID() {
    return m_socket.id;
}

void ZSocket::operator<<(const char *data) {
    std::string output(data);
    operator<<(output);
}

void ZSocket::operator<<(ByteArray &pckt) {
    operator<<(pckt.genPacket());
}

void ZSocket::operator<<(std::string output) {
    pthread_mutex_lock(&m_mutex);
        size_t TotalBytesSend = 0;
        DWORD BytesSend;
        while( TotalBytesSend < output.size() ){
            BytesSend = send(m_socket.id, output.c_str() + TotalBytesSend, output.size() - TotalBytesSend, 0);
            if( BytesSend == SOCKET_ERROR || BytesSend == 0 ){
                std::cerr << "[ZSocket] @ERROR: Failed to send all bytes" << std::endl;
                socket_close();
                break;
            }
            TotalBytesSend += BytesSend;
        }
	pthread_mutex_unlock(&m_mutex);
}

void ZSocket::socket_close() {
    shutdown(m_socket.id,2);
    closesocket(m_socket.id);
    m_connected = false;
    m_created = false;
}

bool ZSocket::isDataReceived() {
    return m_dataReceived;
}

void ZSocket::operator>>(ByteArray &out) {
    //Init vars
    char buffer[CONFIG_ZSOCKET_READ_BUFFER_SIZE];
    int bytesRead;
    //Receive packet
    bytesRead = recv(m_socket.id, &buffer[0], CONFIG_ZSOCKET_READ_BUFFER_SIZE, 0);
    ByteArray output(buffer, bytesRead);
    //Set m_connected flag
    if (bytesRead >= 0) {
        if (bytesRead > 0) {
            m_dataReceived = true;
        } else {
            m_dataReceived = false;
        }
        m_connected = true;
    } else {
        m_connected = false;
        m_dataReceived = false;
    }
    //Return
    out.clear();
    out.append(output);
}

bool ZSocket::isConnected() {
    return m_connected;
}

bool ZSocket::checkIsConnected() {
    int bytesRead = recv(m_socket.id, NULL, 0, 0);
    if (bytesRead == 0) {
        m_connected = true;
    } else {
        m_connected = false;
    }
    return m_connected;
}

#if defined(WIN32)
void ZSocket::_loadWinsock() {
    int error=0;
    //Definition Winsock [WIN32]
    WSADATA init_win32; // Variable permettant de récupérer la structure d'information sur l'initialisation
    // Loading Winsock [WIN32]
    error=WSAStartup(MAKEWORD(2,2),&init_win32);
    if (error!=0) {
        std::cerr << "[ZSocket] @ERROR: ZSocket failed to load Winsock: #" << error << std::endl;
    }
}

void ZSocket::_unloadWinsock() {
    int error=WSACleanup();
    if (error!=0) {
        std::cerr << "[ZSocket] @ERROR: ZSocket failed to cleanup WSA out of memory: " << error << std::endl;
    }
}
#endif
