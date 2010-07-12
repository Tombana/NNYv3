#ifndef H_THREAD
#define H_THREAD

#include <iostream>
#include "ace/Thread.h" 

class Thread {
public:
	Thread();
	~Thread();
   /** This function will create, start a new thread and detach it.
    * The threaded function is the static member threadTask() that inafter will call virtual run()
    * @brief Start a new thread
    */
	virtual void run() = 0;
	bool start();
	void destroy();
	void suspend();
	void resume();
	bool wait();
private:
	static ACE_THR_FUNC_RETURN threadTask(void * param);

	ACE_thread_t   m_thread_id; ///< The thread ID, debugging purpose mostly
	ACE_hthread_t  m_thread_handle; ///< The thread handle (suspend and all)
	int			   m_flag;
	bool		   m_running;
};

/*
TODO: Member functions to implement

static void sleep(unsigned long msecs);

static ACE_thread_t currentId();
static ACE_hthread_t currentHandle();
static Thread * current();

You may need this, it looks interesting:
	//global object - container for Thread class representation of every thread
    //static ACE_TSS<Thread> m_threadStorage;
*/

#endif
