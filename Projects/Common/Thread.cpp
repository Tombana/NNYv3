#include "Thread.h"

Thread::Thread() {
}

Thread::~Thread() {
}

bool Thread::start() {
    //Error var
    bool error = false;
    //Start the thread
    if (pthread_create(&m_thread, NULL, inner_main, this)) {
        std::cerr << "[Thread] @ERROR: pthread_create() failed!" << std::endl;
        error = true;
    }
    //Detach the thread
    if (pthread_detach(m_thread)) {
        std::cerr << "[Thread] @ERROR: pthread_detach() failed!" << std::endl;
        error = true;
    }
    //Return
    return error;
}

bool Thread::stop(bool bForceKill) {
    bool error = false;
    if (bForceKill) {
        if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
            std::cerr << "[Thread] @ERROR: pthread_setcancelstate() failed!" << std::endl;
            error = true;
        }
        if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
            std::cerr << "[Thread] @ERROR: pthread_setcanceltype() failed!" << std::endl;
            error = true;
        }
    }
    if (pthread_cancel(m_thread)) {
        std::cerr << "[Thread] @ERROR: pthread_cancel() failed!" << std::endl;
        error = true;
    }
    return error;
}

void Thread::main() {
    std::cerr << "Thread default main(); please override this function!" << std::endl;
}
