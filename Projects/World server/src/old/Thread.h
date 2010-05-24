#ifndef H_THREAD
#define H_THREAD

#include <iostream>
#include "pthread.h"

/** This class itself is doing pretty much nothing alone. It must be coupled with another
* class to become useful. Notice it's a simplified pthread wrapper that let's you override
* its main() function and let you run it as a thread in-after.
*/
class Thread {
    public:
        Thread(); ///< Default constructor (empty)
        ~Thread(); ///< Default destructor (empty)
        /** This function will create, start a new thread and detach it. The threaded function is the static member inner_main()
        * @brief Start a new thread
        */
        bool start ();
        //TODO: This function is bugged for some reasons
        //bool stop (bool bForceKill = false ); ///< Stops the thread (bugged)
        /** Overrite this method; thread entry point.
        * @note By default it will print a error message if it haven't been overrided.
        */
        virtual void main();

    public:
        /// This is the internal main function called by the pthread library.
        /// This one will actually call our member main() function. I know it's weird but C is.
        static void* inner_main(void* arg) {
            ((Thread*)arg)->main();
            return NULL;
        }

    private:
        pthread_t m_thread; ///< The thread handle
};

#endif
