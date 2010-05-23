#ifndef H_ADDRESS
#define H_ADDRESS

#if defined(WIN32)
	#include <winsock2.h>
	#include <windows.h>
#else
    #include <sys/socket.h>
    #include <sys/types.h>
    //#include <resolv.h>
    //#include <netdb.h>
    //#include <arpa/inet.h>
    #define SOCKADDR_IN    struct sockaddr_in
    #define SOCKADDR       struct sockaddr
#endif

/** In my quest to optimize the server, I noticed how many time this structure
* is accessed everywhere with most of the time crazy syntax quite impossible to
* understand so this class is a \b very simplified wrapper.
*/
class Address {
	public:
		Address(const char *ip, int port); ///< Well our constructor
		~Address();
	protected:
        const SOCKADDR_IN& getSockaddr(); ///< This is an accessor to \a m_infos
	private:
        SOCKADDR_IN m_infos; ///< Basically contains everything related to the connection
};

#endif
