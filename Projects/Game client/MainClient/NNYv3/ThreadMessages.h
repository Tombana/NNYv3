#pragma once

#include <list>
#include <string>
#include "pthread.h"

//
// Tombana's thread message system. Probably crappy and there are probably better systems out there
// A class that needs to receive messages uses CThreadMessages as base class.
// The class can use GetThreadMessage to check for messages.
// Other threads can call SendThreadMessage to send a message.
// Other threads must allocate the message (which must be based on CMessage) and
// the class that receives the message must deallocate it.
//


//Base class for all thread messages
class CMessage{
public:
	CMessage(int MessageID) : ID(MessageID){}
	virtual ~CMessage(void){}
	//The message identifier
	const int ID;
};


//Base class for classes that other threads need to communicate with
class CThreadMessages
{
public:
	CThreadMessages(void);
	virtual ~CThreadMessages(void);

	//When other threads want to notify the this thread of something they call this function
	//Example usage: SendThreadMessage(new CMessageLogin("username", "password"));
	int SendThreadMessage(CMessage* Message);

	//When the message only has an identifier without additional data this might be easier:
	int SendThreadMessage(int Identifier);

protected:
	//Retrieves the next message in the list.
	//Returns zero if there are no messages.
	//VERY IMPORTANT: delete the memory when you're done
	//THE CALLER IS RESPONSIBLE FOR DELETING THE MEMORY!!
	CMessage* GetThreadMessage(void);

private:
	//When other threads notify this thread, the notification will be put in this list
	std::list<CMessage*>	m_MessageQueue;
	pthread_mutex_t			m_message_mutex;
};


//==================================
// All thread message constants
//==================================

//General messages
static const int	Message_Quit		=	100;	//The user pressed close. This does not neccesarily mean that the client should close immediately. There can be a confirmation or something like that.
//To the main thread
static const int	Message_Login		=	1000;	//with Username and Password parameter
static const int	Message_CharSelect	=	1001;
//To the GUI thread
static const int	Message_DisplayLoginScreen	=	2001;	//with RememberedUsername parameter
static const int	Message_DisplayWaitScreen	=	2002;	//with Text parameter
static const int	Message_CloseWaitScreen		=	2003;	//no parameters. Closes any previous wait screen

//==================================
// All thread messages that have parameters
//==================================

//Messages to the main thread

class CMessageLogin : public CMessage{
public:
	CMessageLogin(std::string username, std::string password) : CMessage(Message_Login), Username(username), Password(password) {}
	std::string	Username;
	std::string Password;
};

// Messages to the GUI thread

class CMessageDisplayLoginScreen : public CMessage{
public:
	CMessageDisplayLoginScreen(std::string rememberedusername) : CMessage(Message_DisplayLoginScreen), RememberedUsername(rememberedusername) {}
	std::string	RememberedUsername;
};

class CMessageDisplayWaitScreen : public CMessage{
public:
	CMessageDisplayWaitScreen(std::string text) : CMessage(Message_DisplayWaitScreen), Text(text) {}
	std::string Text;
};