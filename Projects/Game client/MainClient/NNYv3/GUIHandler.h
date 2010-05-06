#pragma once

#include <CEGUI/CEGUI.h>

class CGUIHandler
{
public:
	CGUIHandler(CEGUI::WindowManager *WindowManager);
	~CGUIHandler(void);

	//Creates the basic root window and so on
	void Setup(void);

	//Shows a messagebox with an Ok button.
	//Parameter WindowName:
	// - can be used to give the window a name so it can be looked up later.
	// - if used then there can only be one instance of the messagebox, if the function
	//   is called while there is still a window active with that name, nothing happens and it will return 0
	CEGUI::Window* MsgBox(std::string Text, std::string Title, std::string WindowName = "");

	bool QuitBtnClick(const CEGUI::EventArgs &e);
	bool MessageBoxBtnOkClick(const CEGUI::EventArgs &e);

	//===================
	// Login section
	//===================
	int DisplayLoginScreen(void);
	bool LoginBtnClick(const CEGUI::EventArgs &e);
	bool AboutBtnClick(const CEGUI::EventArgs &e);


private:
	CEGUI::WindowManager	*mWindowManager;
	CEGUI::Window			*mRootWindow;
};
