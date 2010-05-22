#ifndef H_THREAD
#define H_THREAD

#include <iostream>
#include "pthread.h"

class Thread {
    public:
        Thread(); ///< Default constructor
        ~Thread(); ///< Default destructor
        bool start (); ///< Start the thread with arg
        //TODO: This function is bugged for some reasons
        //bool stop (bool bForceKill = false ); ///< Stops the thread (bugged)
        virtual void main();     ///< Overrite this method; thread entry point.

    public:
        static void* inner_main(void* arg) {
            ((Thread*)arg)->main();
            return NULL;
        }

    private:
        pthread_t m_thread; ///< The thread handle
};

#endif
