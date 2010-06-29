#pragma once

#include "Structures.h"
#include "Callback.h"

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
static const int	Message_RealmLoaded	=	 999;	//parameters: none
static const int	Message_ConnectWorld=	1000;	//parameters: int ServerIndex
static const int	Message_WorldConnected=	1001;	//parameters: none
static const int	Message_Login		=	1002;	//parameters: Username, Password
static const int	Message_KickAccount	=	1003;	//parameters: bool (true kicks and continues, false will close connection)
static const int	Message_CharSelect	=	1004;	//parameters: slot
//
//To the GUI thread
//
static const int	Message_MsgBox				=	2000;	//parameters: text, title, buttons(optional), callback(optional), windowname(optional)
static const int	Message_DisplayWaitScreen	=	2001;	//parameters: text	(Any previous wait screens are closed)
static const int	Message_CloseWaitScreen		=	2002;	//parameters: none	(Closes any previous wait screen)
//Login procedure
static const int	Message_NoWorld				=	2009;	//parameters: none
static const int	Message_DisplayWorldSelect	=	2010;	//parameters: vector<WORLDSERVER>& Serverlist
static const int	Message_DisplayLoginScreen	=	2012;	//parameters: RememberedUsername
static const int	Message_LoginResponse		=	2013;	//parameters: ACK_ code
static const int	Message_DisplayCharSelect	=	2014;	//parameters: vector<CharacterInfo>, last_selected_char

//For the Message_MsgBox, for the button parameter
static const int	MsgBoxBtnsOk	= 0;
static const int	MsgBoxBtnsYesNo	= 1;

//==================================
// All thread messages that have parameters
//==================================

//
//Messages to the main thread
//

class CMessageParamsConnectWorld : public CMessageParams{
public:
	CMessageParamsConnectWorld(int serverindex) : CMessageParams(Message_ConnectWorld), ServerIndex(serverindex) {}
	int ServerIndex;
};

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

class CMessageParamsDisplayWorldSelect : public CMessageParams{
public:
	CMessageParamsDisplayWorldSelect(std::vector<WORLDSERVER>& servers) : CMessageParams(Message_DisplayWorldSelect), Servers(servers) {}
	std::vector<WORLDSERVER>& Servers;
};

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