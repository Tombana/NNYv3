#include "ThreadMessages.h"

CThreadMessages::CThreadMessages(void) :
	//m_MessageQueue(), m_message_mutex(PTHREAD_MUTEX_INITIALIZER)
	m_Read(0), m_Write(0)
{
	for( int i = 0; i < Size; ++i ){
		m_Data[i].ID = 0;
		m_Data[i].params = 0;
	}
}

CThreadMessages::~CThreadMessages(void)
{
	MESSAGE msg = {0};
	while( GetThreadMessage(msg) ){
		if( msg.params ) delete msg.params;
	}
}

bool CThreadMessages::SendThreadMessage(PCMessageParams Message)
{
	if( !Message ) return false;
	MESSAGE msg = {0};
	msg.ID = Message->ID;
	msg.params = Message;
	return SendThreadMessage(msg);
}

bool CThreadMessages::SendThreadMessage(int Identifier)
{
	if( !Identifier ) return false;
	MESSAGE msg = {0};
	msg.ID = Identifier;
	return SendThreadMessage(msg);
}

bool CThreadMessages::SendThreadMessage(MESSAGE& Message)
{
	int nextElement = (m_Write + 1) % Size;
	if(nextElement != m_Read) //If the queue is not full
	{
		m_Data[m_Write].params = Message.params;
		m_Data[m_Write].ID = Message.ID;
		m_Write = nextElement;
		return true;
	}else
		return false;
}

bool CThreadMessages::GetThreadMessage(MESSAGE& Message)
{
	if(m_Read == m_Write) return false; //If queue is empty
	int nextElement = (m_Read + 1) % Size;
	Message.params = m_Data[m_Read].params;
	Message.ID = m_Data[m_Read].ID;
	m_Read = nextElement;
	return true;
}
