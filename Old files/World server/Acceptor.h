#include "config_static.h"
#include <iostream>

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
#endif

class Acceptor {
	public:
        /**
         * @brief Create a server socket.
         * @param [addr] Attach addr for binding.
         * @remarks Create a socket handle and begin to listen from client connection.
         */
        Acceptor(const char *ip, int port);
		~Acceptor();
	protected:
	private:
        SOCKET m_socket; ///< Socket the server will bind/listen on
};

