#ifndef H_THREADREALMCONNECTOR
#define H_THREADREALMCONNECTOR

#include "Thread.h"
#include <iostream>
#include "protocol.h"
#include "sharedModels.h"
#include "ZSocket.h"
#include "config.hpp"

class ThreadRealmConnector : public Thread {
    public:
        ThreadRealmConnector();
    protected:
        virtual void main();
    private:
        ZSocket         m_socket;
        unsigned short  m_errorCounter;
};

#endif
