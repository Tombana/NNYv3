#include "ThreadMessages.h"

CThreadMessages::CThreadMessages(void) :
	//m_MessageQueue(), m_message_mutex(PTHREAD_MUTEX_INITIALIZER)
	m_Read(0), m_Write(0)
{
	for( int i = 0; i < Size; ++i ) m_Data[i] = 0;
}

CThreadMessages::~CThreadMessages(void)
{
	CMessage* msg;
	while( (GetThreadMessage(msg)) ) delete msg;
}

bool CThreadMessages::SendThreadMessage(PCMessage Message)
{
	if( !Message ) return false;
	int nextElement = (m_Write + 1) % Size;
	if(nextElement != m_Read) //If the queue is not full
	{
		m_Data[m_Write] = Message;
		m_Write = nextElement;
		return true;
	}else
		return false;
	//pthread_mutex_lock(&m_message_mutex);
	//m_MessageQueue.push(Message);
	//pthread_mutex_unlock(&m_message_mutex);
}

bool CThreadMessages::SendThreadMessage(int Identifier)
{
	if( !Identifier ) return false;
	return SendThreadMessage(new CMessage(Identifier));
}

bool CThreadMessages::GetThreadMessage(PCMessage& Message)
{
	if(m_Read == m_Write) return false; //If queue is empty
	int nextElement = (m_Read + 1) % Size;
	Message = m_Data[m_Read];
	m_Read = nextElement;
	return true;
	//if( m_MessageQueue.size() == 0 ) return 0;
	//pthread_mutex_lock(&m_message_mutex);
	//CMessage *msg = m_MessageQueue.front(); //Get the message from the queue
	//m_MessageQueue.pop(); //Remove the message from the queue
	//pthread_mutex_unlock(&m_message_mutex);
	//return msg;
}
