#include "Acceptor.h"

Acceptor::Acceptor(const char *ip, int port) {
    //Address infos
    SOCKADDR_IN temp_address;
    temp_address.sin_family      = AF_INET;
    temp_address.sin_port        = htons(port);
    if (ip != NULL) {
        temp_address.sin_addr.s_addr = inet_addr(ip);
    } else {
        temp_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    //Create a socket
    m_socket = socket(AF_INET,SOCK_STREAM,0);
    //Check for errors
    if (m_socket==INVALID_SOCKET) {
        std::cout << "[Acceptor] @ERROR: Failed to create a socket!" << std::endl;
    }
    //Bind the socket & check for errors
    if (bind(m_socket,(struct sockaddr *)&temp_address,sizeof(temp_address)) != 0) {
        std::cout << "[Acceptor] @ERROR: Unable to bind the socket on the port requested!" << std::endl;
    }
    //Listen socket
    listen(m_socket, CONFIG_ACCEPTOR_MAX_PENDING_CONNECTION); //max connections pending
}

Acceptor::~Acceptor() {
}
