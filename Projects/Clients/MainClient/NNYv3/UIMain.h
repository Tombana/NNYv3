#pragma once

#include "Thread.h"
#include "ThreadMessages.h"
#include "InputHandler.h"
#include "GUIHandler.h"
#include <Ogre.h>
#include "ConsoleOverlay.h"
#include "WorldManager.h"

class CUIMain : public Ogre::FrameListener, public CThreadMessages, public Thread
{
public:
	CUIMain(void);
	~CUIMain(void);

	static CUIMain& getSingleton(){ return *mSingleton; }
	static CUIMain* getSingletonPtr(){ return mSingleton; }

	//Called form main thread: To launch the whole GUI
	int StartUI(void);
	int WaitForExit(void); //When the main thread is done and this thread is not yet done.

	void ToggleConsole(void){ mShowConsole = !mShowConsole; mConsoleOverlay->Show(mShowConsole); }

private:
	//Singleton
	static CUIMain	*mSingleton;

	bool		Started;
	//================
	// Thread related
	//================
	void run(void){ UIThread(); }
	int UIThread(void);

	//==============
	// Ogre related
	//==============
	Ogre::Root			*mRoot;
	Ogre::Camera		*mCamera;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RenderWindow	*mWindow;
	Ogre::RaySceneQuery	*mRaySceneQuery;
	CInputHandler		*mInputHandler;
	// CEGUI related
	CGUIHandler			*mGUIHandler;

	//This will be called every frame
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	int SetupOgre(void);
	int CleanupOgre(void);
	int LoadWorld(void);

	//Console overlay
	bool				mShowConsole;
	ConsoleOverlay		*mConsoleOverlay;

	//=============
	// Login section
	//=============
	bool MsgBoxKickCallback(void* Param);

	//================
	// InGame related
	//================
	CWorldManager mWorld;
};
