#include "Thread.h"

Thread::Thread() {
	m_thread_id = 0;
	m_thread_handle = 0;
	m_flag = 0;
	m_running = false;
}

Thread::~Thread() {
}

bool Thread::wait() {
	ACE_THR_FUNC_RETURN temp_func = ACE_THR_FUNC_RETURN(-1);
    int result = ACE_Thread::join(m_thread_handle, &temp_func);

    m_thread_id = 0;
    m_thread_handle = 0;

    return (result == 0);
}

void Thread::destroy() {
    if (ACE_Thread::kill(m_thread_id, -1) != 0) return;

	m_thread_id = 0;
    m_thread_handle = 0;
	m_flag = 0;
	m_running = false;
}

void Thread::suspend() {
	if (m_flag > 0) { //check if someone tried to resume the thread earlier (even when active)
		m_flag--; //if so, don't go to sleep then; simply decrement the flag
	} else {
		if (m_running) {
			m_running = false;
			ACE_Thread::suspend(m_thread_handle);
		}
	}
}

void Thread::resume() {
	if (m_flag < 0) { //check if someone tried to suspend the thread earlier (when already suspended)
		m_flag++; //if so, don't resume the thread; simply increment the flag
	} else {
		if (!m_running) {
			m_running = true;
			ACE_Thread::resume(m_thread_handle);
		}
	}
}

bool Thread::start() {
	if (!m_running) { //start the thread only if it isn't already running
		if (ACE_Thread::spawn(&Thread::threadTask, (void*)this, 0, &m_thread_id, &m_thread_handle) == 0) {
			m_running = true;
			return true;
		} else {
			std::cout << "@ERROR: Thread: ACE_Thread::spawn() failed!" << std::endl;
			return false;
		}
	} else { //display an error otherwise
		std::cout << "@ERROR: Thread: Thread is already running!" << std::endl;
		return false;
	}
}

ACE_THR_FUNC_RETURN Thread::threadTask(void * param) {
    Thread * task = (Thread*)param;
    task->run();
    return (ACE_THR_FUNC_RETURN)0;
}