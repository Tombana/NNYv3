#pragma once

// Taken from
// http://msmvps.com/blogs/vandooren/archive/2007/01/05/creating-a-thread-safe-producer-consumer-queue-in-c-without-using-locks.aspx
// IMPORTANT: there may be only one reader thread and one writer thread
// The reading thread class uses CThreadMessages as base class.
// The reading thread can use GetThreadMessage to check for messages.
// The writing thread can call SendThreadMessage to send a message.
// The writing thread must allocate the message (which must be based on CMessageParams) and
// the reading thread must deallocate it.
//

#include <vector>
#include <string>

//Base class for all thread messages that have parameters
typedef class CMessageParams{
public:
	CMessageParams(int MessageID) : ID(MessageID){}
	virtual ~CMessageParams(void){}
	//The message identifier
	const int ID;
} *PCMessageParams;

//If a message has parameters, it has a pointer, otherwise, it is zero.
typedef struct MESSAGE{
	int ID;
	PCMessageParams params;
} *PMESSAGE;


//Base class for classes that other threads need to communicate with
class CThreadMessages
{
public:
	CThreadMessages(void);
	virtual ~CThreadMessages(void);

	//
	//Called by the writing thread to send a message
	//Returns zero if the queue was full (message not placed in queue)
	//
	//When the message only has an identifier without additional parameters:
	bool SendThreadMessage(int Identifier);
	//When the caller has already constructed the MESSAGE
	bool SendThreadMessage(MESSAGE& Message);
	//When the message has parameters:
	//Example usage: SendThreadMessage(new CMessageParamsLogin("username", "password"));
	bool SendThreadMessage(PCMessageParams Message);

protected:
	//For the reading thread: retrieves the next message in the list.
	//Returns zero if there are no messages.
	//VERY IMPORTANT: deallocate the message.params when it is not zero
	//THE CALLER IS RESPONSIBLE FOR DELETING THE MEMORY!!
	//So after parsing the message:
	//	if( Message.params ) delete Message.params;
	bool GetThreadMessage(MESSAGE& Message);

private:
	//When other threads notify this thread, the notification will be put in this list
	volatile int m_Read;
	volatile int m_Write;
	static const int Size = 20;
	volatile MESSAGE m_Data[Size];
};