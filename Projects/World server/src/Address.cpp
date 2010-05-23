#include "Address.h"

/** This only support \a ip and \a port for now
* \param ip Is of course the destination IP. It must be set to \b NULL (wich is 0.0.0.0) when used by the Accessor class.
* \param port Is the destination port or the port the Accessor class will listen on.
*/
Address::Address(const char *ip, int port) {
    m_infos.sin_family      = AF_INET;
    m_infos.sin_port        = htons(port);
    if (host != NULL) {
        m_infos.sin_addr.s_addr = inet_addr(ip);
    } else {
        m_infos.sin_addr.s_addr = htonl(INADDR_ANY);
    }
}

Address::~Address() {
}

const SOCKADDR_IN& Address::getSockaddr() {
    return m_infos;
}
