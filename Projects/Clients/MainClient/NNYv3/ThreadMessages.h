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
#include "Structures.h"
#include "Callback.h"


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
class CMessageParamsLogin : public CMessageParams{
public:
	CMessageParamsLogin(std::string username, std::string password) : CMessageParams(Message_Login),
		Username(username), Password(password) {}
	std::string	Username;
	std::string Password;
};

class CMessageParamsKickAccount : public CMessageParams{
public:
	CMessageParamsKickAccount(bool Kick) : CMessageParams(Message_KickAccount), DoKick(Kick) {};
	bool DoKick;
};

//
// Messages to the GUI thread
//
class CMessageParamsMsgBox : public CMessageParams{ //See Callback.h for explanation on the callback parameter
public:
	CMessageParamsMsgBox(std::string text, std::string title, int buttons = MsgBoxBtnsOk, CallbackFunction* callback = 0, std::string windowname = "") : CMessageParams(Message_MsgBox),
		Text(text), Title(title), Buttons(buttons), Callback(callback), WindowName(windowname) {}
	std::string Text;
	std::string	Title;
	int Buttons;
	CallbackFunction* Callback;
	std::string WindowName;
};

class CMessageParamsDisplayWaitScreen : public CMessageParams{
public:
	CMessageParamsDisplayWaitScreen(std::string text) : CMessageParams(Message_DisplayWaitScreen), Text(text) {}
	std::string Text;
};

// Login procedure

class CMessageParamsDisplayLoginScreen : public CMessageParams{
public:
	CMessageParamsDisplayLoginScreen(std::string rememberedusername) : CMessageParams(Message_DisplayLoginScreen), RememberedUsername(rememberedusername) {}
	std::string	RememberedUsername;
};

class CMessageParamsLoginResponse : public CMessageParams{
public:
	CMessageParamsLoginResponse(int code) : CMessageParams(Message_LoginResponse), Code(code) {}
	int Code;
};

class CMessageParamsDisplayCharSelect : public CMessageParams{
public:
	CMessageParamsDisplayCharSelect(const std::vector<CharacterInfo>& charinfo, int LastSelectedChar = 0) : CMessageParams(Message_DisplayCharSelect),
		CharInfo(charinfo), SelectedChar(LastSelectedChar) {}
	std::vector<CharacterInfo> CharInfo;
	int SelectedChar;
};