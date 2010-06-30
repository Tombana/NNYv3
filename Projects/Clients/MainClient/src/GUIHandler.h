#pragma once

//=================================
//Tombana: this is my own define.
//(It does not have any effect in the CEGUI header files)
//I found multiple tutorials with CEGUI
//and some used the newest version of CEGUI
//and some used older versions.
//Since the SDK comes with an older version
//I will just use the old version because otherwise
//you have to install the CEGUI SDK seperately and so on.
//If you use a newer version of CEGUI then comment out the define.
//For the new CEGUI you have to link with
//CEGUIBase_d.lib and CEGUIOgreRenderer_d.lib
//(remove _d for release mode libs)
//For the old CEGUI you have to link with
//CEGUIBase_d.lib OgreGUIRenderer_d.lib
//(remove _d for release mode libs)
#define OLD_CEGUI
//=================================
#ifdef OLD_CEGUI
#include <CEGUI/CEGUI.h>
#include "Ogre/OgreCEGUIRenderer.h"
#else
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#endif

#include "Callback.h"
#include "Structures.h"

class CGUIHandler
{
public:
	CGUIHandler(Ogre::RenderWindow *Window, Ogre::SceneManager *SceneMgr);
	~CGUIHandler(void);

	//Creates and shows a message box.
	//Parameter Buttons: one of the following
	static const int	MsgBoxBtnsOk	= 0;
	static const int	MsgBoxBtnsYesNo	= 1;
	//Parameter CallBack:
	// - usage example: new MemberCallbackFunction(&CUIMain::SomeCallback, m_uimain)
	// - If set, it is the function that will be called when the user presses the Ok button.
	// - The parameter passed to the callback function is an int that specifies which button was pressed.
	static const int MsgBoxBtnCancel= 0;
	static const int MsgBoxBtnOk	= 1;
	static const int MsgBoxBtnYes	= 2;
	static const int MsgBoxBtnNo	= 3;
	// - if the callback returns true the MessageBox will be closed
	// - if the callback returns false then nothing happens.
	//Parameter WindowName:
	// - can be used to give the window a name so it can be looked up later.
	// - if used then there can only be one instance of the messagebox, if the function
	//   is called while there is still a window active with that name, nothing happens and it will return 0
	CEGUI::Window* MsgBox(std::string Text, std::string Title, int Buttons = MsgBoxBtnsOk, CallbackFunction* Callback = 0, std::string WindowName = "");

	//When logging in and waiting for the connection there is usually a wait screen
	//DisplayWaitScreen will display the wait screen with the given Text
	//Only one wait screen at a time can be displayed, if a waitscreen was
	//already being displayed, it is removed and the new one is made.
	//It can be closed with the CloseWaitScreen function
	int DisplayWaitScreen(std::string Text);
	int CloseWaitScreen(void);

	//===================
	// Login section
	//===================
	int DisplayWorldSelect(const std::vector<WORLDSERVER>& Servers);
	int DisplayWorldSelect(bool Enabled); //To enable/disable it
	int CloseWorldSelect(void);
	int DisplayLoginScreen(const std::string& RememberedUsername);
	int CloseLoginScreen(void);

private:
	// General CEGUI variables
#ifdef OLD_CEGUI
	CEGUI::OgreCEGUIRenderer	*mGUIRenderer;
#else
	CEGUI::OgreRenderer			*mGUIRenderer;
#endif
	CEGUI::System			*mGUISystem;
	CEGUI::WindowManager	*mWindowManager;
	CEGUI::Window			*mRootWindow;

	bool QuitBtnClick(const CEGUI::EventArgs &e);

	static const int DefaultButtonWidth = 100;
	static const int DefaultButtonHeight = 30;
	const CEGUI::UVector2 DefaultButtonSize;
	//==========
	//Message boxes
	//==========
	//Message box handlers in CEGUI format. They should all call MessageBoxClick (see at private section) with the button id as parameter (see above)
	bool MessageBoxBtnOkClick(const CEGUI::EventArgs &e);
	bool MessageBoxBtnYesClick(const CEGUI::EventArgs &e);
	bool MessageBoxBtnNoClick(const CEGUI::EventArgs &e);
	//The CEGUI callbaks should call this. This function will call the callback if specified.
	bool MessageBoxClick(const CEGUI::EventArgs &e, int Button);

	//==========
	// The general waiting screen
	//==========
	CEGUI::Window			*WaitScreen;
	CEGUI::Window			*WaitScreenLabel;
	int	LoadWaitScreen(void); //Creates the (hidden) wait screen used by DisplayWaitScreen and CloseWaitScreen

	//==========
	//Login section
	//==========
	bool WorldSelectBtnClick(const CEGUI::EventArgs &e);
	bool LoginBtnClick(const CEGUI::EventArgs &e);
	bool AboutBtnClick(const CEGUI::EventArgs &e);
};
