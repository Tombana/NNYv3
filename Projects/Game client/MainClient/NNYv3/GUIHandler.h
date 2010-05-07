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
#include "OgreCEGUIRenderer.h"
#else
#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#endif


class CGUIHandler
{
public:
	CGUIHandler(Ogre::RenderWindow *Window, Ogre::SceneManager *SceneMgr);
	~CGUIHandler(void);

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
#ifdef OLD_CEGUI
	CEGUI::OgreCEGUIRenderer	*mGUIRenderer;
#else
	CEGUI::OgreRenderer			*mGUIRenderer;
#endif
	CEGUI::System			*mGUISystem;
	CEGUI::WindowManager	*mWindowManager;
	CEGUI::Window			*mRootWindow;
};
