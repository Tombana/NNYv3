#include "threadPoolHandler.h"

extern  SOCKET          g_threadStartupData_socketID; //exported from main()
extern  pthread_mutex_t g_threadPool_mutex; //exported from main()
extern  pthread_cond_t  g_threadPool_cond; //exported from main()
extern  int             g_threadPool_counter_job; //exported from main()
extern  int             g_threadPool_counter_thread; //exported from main()
extern  pthread_mutex_t g_threadPool_counter_mutex; //exported from main()

void *threadPoolHandler (void *ptr) {
    //A loop so we can re-use the thread as many time as we need :)
    while (true) {
        //Waiting for focus; usually acquired when there's a new socket.
        //Because we are using many global variables in the next lines,
        //we must acquire the lock on a mutex first to prevent errors.
        pthread_mutex_lock(&g_threadPool_mutex);

        //No jobs, pause the thread and wait for a signal
        pthread_cond_wait(&g_threadPool_cond, &g_threadPool_mutex); //pause thread

        //====================================================
        //        IMPORTANT VARS TO COPY LOCALLY!
        //          [all g_threadStartupData_]
        //====================================================
        ZSocket j_socket(g_threadStartupData_socketID);

        //====================================================
        //                  RELEASE LOCK
        // Release focus/lock; normal processing from now :)
        //====================================================
        pthread_mutex_unlock(&g_threadPool_mutex);

        //====================================================
        //                    JOB HERE
        //====================================================

        sleep(20);
        j_socket.socket_close();

        //====================================================
        //                  JOB IS DONE :)
        //====================================================
        pthread_mutex_lock(&g_threadPool_counter_mutex);
        g_threadPool_counter_job--;
        #if CONFIG_VERBOSE >= 2
            std::cerr << "[threadPoolHandler] " << g_threadPool_counter_job << " active jobs in " <<
            g_threadPool_counter_thread << " threads." << std::endl;
        #endif
        pthread_mutex_unlock(&g_threadPool_counter_mutex);
        //TODO (NitriX#): Should the thread be terminated now?
        //ALGORYTM HERE, WOHOO!
        //....
        //if() {
        //pthread_mutex_lock(&g_threadPool_counter_mutex);
        //g_threadPool_counter_thread--;
        //pthread_mutex_unlock(&g_threadPool_counter_mutex);
        //break; //break the loop; destroy thread
        //}
    }
}
