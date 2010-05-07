#include "ThreadMessages.h"

CThreadMessages::CThreadMessages(void) : m_MessageQueue(), m_message_mutex(PTHREAD_MUTEX_INITIALIZER)
{
}

CThreadMessages::~CThreadMessages(void)
{
	pthread_mutex_lock(&m_message_mutex);
	for( std::list<CMessage*>::iterator it = m_MessageQueue.begin(); it != m_MessageQueue.end(); ++it ){
		delete *it;
	}
	pthread_mutex_unlock(&m_message_mutex);
}

int CThreadMessages::SendThreadMessage(CMessage* Message)
{
	if( Message ){
		pthread_mutex_lock(&m_message_mutex);
		m_MessageQueue.push_back(Message);
		pthread_mutex_unlock(&m_message_mutex);
	}
	return 1;
}

int CThreadMessages::SendThreadMessage(int Identifier)
{
	if( Identifier ){
		SendThreadMessage(new CMessage(Identifier));
	}
	return 1;
}

CMessage* CThreadMessages::GetThreadMessage(void)
{
	if( m_MessageQueue.size() == 0 ) return 0;
	pthread_mutex_lock(&m_message_mutex);
	CMessage *msg = m_MessageQueue.front(); //Get the message from the queue
	m_MessageQueue.pop_front(); //Remove the message from the queue
	pthread_mutex_unlock(&m_message_mutex);
	return msg;
}
