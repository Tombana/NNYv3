#pragma once

// Taken from
// http://msmvps.com/blogs/vandooren/archive/2007/01/05/creating-a-thread-safe-producer-consumer-queue-in-c-without-using-locks.aspx
// IMPORTANT: there may be only one reader thread and one writer thread
// The reading thread class uses CThreadMessages as base class.
// The reading thread can use GetThreadMessage to check for messages.
// The writing thread can call SendThreadMessage to send a message.
// The writing thread must allocate the message (which must be based on CMessage) and
// the reading thread must deallocate it.
//


#include <vector>
#include <string>
#include "Structures.h"
#include "Callback.h"


//Base class for all thread messages
class CMessage{
public:
	CMessage(int MessageID) : ID(MessageID){}
	virtual ~CMessage(void){}
	//The message identifier
	const int ID;
};

typedef CMessage* PCMessage;


//Base class for classes that other threads need to communicate with
class CThreadMessages
{
public:
	CThreadMessages(void);
	virtual ~CThreadMessages(void);

	//Called by the writing thread
	//Returns zero if the queue was full (message not placed in queue)
	//Example usage: SendThreadMessage(new CMessageLogin("username", "password"));
	bool SendThreadMessage(PCMessage Message);
	//When the message only has an identifier without additional data this might be easier:
	bool SendThreadMessage(int Identifier);

protected:
	//For the reading thread: retrieves the next message in the list.
	//Returns zero if there are no messages.
	//VERY IMPORTANT: delete the memory when you're done
	//THE CALLER IS RESPONSIBLE FOR DELETING THE MEMORY!!
	bool GetThreadMessage(PCMessage& Message);

private:
	//When other threads notify this thread, the notification will be put in this list
	//std::queue<CMessage*>	m_MessageQueue;
	//pthread_mutex_t			m_message_mutex;
	volatile int m_Read;
	volatile int m_Write;
	static const int Size = 20;
	volatile PCMessage m_Data[Size];
};



//==================================
// All thread message constants
//==================================

//
//General messages
//
static const int	Message_Quit		=	100;	//The user pressed close. This does not neccesarily mean that the client should close immediately. There can be a confirmation or something like that.
//
//To the main thread
//
static const int	Message_Login		=	1000;	//parameters: Username, Password
static const int	Message_KickAccount	=	1001;	//parameters: bool (true kicks and continues, false will close connection)
static const int	Message_CharSelect	=	1002;	//parameters: slot
//
//To the GUI thread
//
static const int	Message_MsgBox				=	2000;	//parameters: text, title, buttons(optional), callback(optional), windowname(optional)
static const int	Message_DisplayWaitScreen	=	2001;	//parameters: text	(Any previous wait screens are closed)
static const int	Message_CloseWaitScreen		=	2002;	//parameters: none	(Closes any previous wait screen)
//Login procedure
static const int	Message_NoWorld				=	2009;	//parameters: none
static const int	Message_DisplayLoginScreen	=	2010;	//parameters: RememberedUsername
static const int	Message_LoginResponse		=	2011;	//parameters: ACK_ code
static const int	Message_DisplayCharSelect	=	2012;	//parameters: vector<CharacterInfo>, last_selected_char

//For the Message_MsgBox, for the button parameter
static const int	MsgBoxBtnsOk	= 0;
static const int	MsgBoxBtnsYesNo	= 1;

//==================================
// All thread messages that have parameters
//==================================

//
//Messages to the main thread
//
class CMessageLogin : public CMessage{
public:
	CMessageLogin(std::string username, std::string password) : CMessage(Message_Login),
		Username(username), Password(password) {}
	std::string	Username;
	std::string Password;
};

class CMessageKickAccount : public CMessage{
public:
	CMessageKickAccount(bool Kick) : CMessage(Message_KickAccount), DoKick(Kick) {};
	bool DoKick;
};

//
// Messages to the GUI thread
//
class CMessageMsgBox : public CMessage{ //See Callback.h for explanation on the callback parameter
public:
	CMessageMsgBox(std::string text, std::string title, int buttons = MsgBoxBtnsOk, CallbackFunction* callback = 0, std::string windowname = "") : CMessage(Message_MsgBox),
		Text(text), Title(title), Buttons(buttons), Callback(callback), WindowName(windowname) {}
	std::string Text;
	std::string	Title;
	int Buttons;
	CallbackFunction* Callback;
	std::string WindowName;
};

class CMessageDisplayWaitScreen : public CMessage{
public:
	CMessageDisplayWaitScreen(std::string text) : CMessage(Message_DisplayWaitScreen), Text(text) {}
	std::string Text;
};

// Login procedure

class CMessageDisplayLoginScreen : public CMessage{
public:
	CMessageDisplayLoginScreen(std::string rememberedusername) : CMessage(Message_DisplayLoginScreen), RememberedUsername(rememberedusername) {}
	std::string	RememberedUsername;
};

class CMessageLoginResponse : public CMessage{
public:
	CMessageLoginResponse(int code) : CMessage(Message_LoginResponse), Code(code) {}
	int Code;
};

class CMessageDisplayCharSelect : public CMessage{
public:
	CMessageDisplayCharSelect(const std::vector<CharacterInfo>& charinfo, int LastSelectedChar = 0) : CMessage(Message_DisplayCharSelect),
		CharInfo(charinfo), SelectedChar(LastSelectedChar) {}
	std::vector<CharacterInfo> CharInfo;
	int SelectedChar;
};