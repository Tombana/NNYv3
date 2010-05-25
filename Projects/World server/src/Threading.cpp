#include "Threading.h"

Runnable::Runnable() {
}

Runnable::~Runnable() {
}

Thread::Thread() {
}

Thread::Thread(Runnable* instance) : m_task(instance), m_thread_id(0), m_thread_handle(0) {
    //start();
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
    if (ACE_Thread::kill(m_thread_id, -1) != 0)
        return;

    m_thread_id = 0;
    m_thread_handle = 0;
}

void Thread::suspend() {
    ACE_Thread::suspend(m_thread_handle);
}

void Thread::resume() {
    ACE_Thread::resume(m_thread_handle);
}

bool Thread::start() {
	if (ACE_Thread::spawn(&Thread::threadTask, (void*)m_task, 0, &m_thread_id, &m_thread_handle) == 0) {
		return true;
	} else {
		std::cout << "[Threading] @ERROR: ACE_Thread::spawn() failed!" << std::endl;
		return false;
	}
}

ACE_THR_FUNC_RETURN Thread::threadTask(void * param) {
    Runnable * task = (Runnable*)param;
    task->run();
    return (ACE_THR_FUNC_RETURN)0;
}

/*
void Runnable::main() {
    std::cout << "Thread default main(); please override this function!" << std::endl;
}
*/