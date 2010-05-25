#ifndef H_THREADING
#define H_THREADING

#include <iostream>
#include "ace/Thread.h"

/** This class itself is doing pretty much nothing alone. It must be coupled with another
* class to become useful. Notice it's a simplified pthread wrapper that let's you override
* its main() function and let you run it as a thread in-after.
*/
class Runnable {
    public:
        Runnable(); ///< Default constructor (empty)
        ~Runnable(); ///< Default destructor (empty)
       /** Overrite this method; thread entry point.
        * @note By default it will print a error message if it haven't been overrided.
        */
        //virtual void main();
		virtual void run() = 0;
};

//TODO: Reference counting, see main as a remember why

class Thread {
public:
	Thread();
	explicit Thread(Runnable* instance);
	~Thread();
   /** This function will create, start a new thread and detach it. The threaded function is the static member inner_main()
    * @brief Start a new thread
    */
	bool start();
	void destroy();
	void suspend();
	void resume();
	bool wait();
private:
	static ACE_THR_FUNC_RETURN threadTask(void * param);

	Runnable	  *m_task;
	ACE_Thread     m_thread; ///< The ACE_Thread object
	ACE_thread_t   m_thread_id; ///< The thread ID, debugging purpose mostly
	ACE_hthread_t  m_thread_handle; ///< The thread handle (suspend and all)
};

	    /*
	bool wait();
    void destroy();

    void suspend();
    void resume();

	static void sleep(unsigned long msecs);
	*/

	//static ACE_thread_t currentId();
    //static ACE_hthread_t currentHandle();
	//static Thread * current();

#endif
