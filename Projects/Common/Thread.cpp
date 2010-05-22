#include "Thread.h"

Thread::Thread() {
}

Thread::~Thread() {
}

/// This function will create, start a new thread and detach it. The threaded function is the static member inner_main()
bool Thread::start() {
    //Error var
    bool error = false;
    //Start the thread
    if (pthread_create(&m_thread, NULL, inner_main, this)) {
        std::cout << "[Thread] @ERROR: pthread_create() failed!" << std::endl;
        error = true;
    }
    //Detach the thread
    if (pthread_detach(m_thread)) {
        std::cout << "[Thread] @ERROR: pthread_detach() failed!" << std::endl;
        error = true;
    }
    //Return
    return error;
}

/*
bool Thread::stop(bool bForceKill) {
    bool error = false;
    if (bForceKill) {
        if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
            std::cout << "[Thread] @ERROR: pthread_setcancelstate() failed!" << std::endl;
            error = true;
        }
        if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
            std::cout << "[Thread] @ERROR: pthread_setcanceltype() failed!" << std::endl;
            error = true;
        }
    }
    if (pthread_cancel(m_thread)) {
        std::cout << "[Thread] @ERROR: pthread_cancel() failed!" << std::endl;
        error = true;
    }
    return error;
}
*/

/// By default it will print a error message if it haven't been overrided.
void Thread::main() {
    std::cout << "Thread default main(); please override this function!" << std::endl;
}
