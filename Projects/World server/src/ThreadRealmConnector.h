#ifndef H_THREADREALMCONNECTOR
#define H_THREADREALMCONNECTOR

#include "Thread.h"
#include <iostream>
#include "protocol.h"
#include "sharedModels.h"
#include "ZSocket.h"
#include "config.hpp"

/** The realm connector' job is to connect at the realm server and send our CONFIG_SERVER_ID + CONFIG_SERVER_KEY to change our `online` flag.
* Then it also read the ACK that is telling us if we are online or not.
*/
class ThreadRealmConnector : public Thread {
    public:
        ThreadRealmConnector();
    protected:
        virtual void main(); ///< This override parent member function -> Thread::main()
    private:
        ZSocket         m_socket; ///< The socket that will be used to connect at the realm server
        unsigned short  m_errorCounter; ///< An error counter to pause the thread if the realm is unreachable for too long (preventing spin-lock)
};

#endif
