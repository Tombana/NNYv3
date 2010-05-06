#pragma once

#include "pthread.h"
#include "InputHandler.h"
#include "GUIHandler.h"
#include <Ogre.h>

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

class CUIMain : public Ogre::FrameListener
{
public:
	CUIMain(void);
	~CUIMain(void);

	static CUIMain& getSingleton(){ return *mSingleton; }
	static CUIMain* getSingletonPtr(){ return mSingleton; }

	//Called form main thread: To launch the whole GUI
	int StartUI(void);
	int WaitForExit(void); //When the main thread is done and this thread is not yet done.
	//Called from main thread: When the main thread wants to notify the gui thread
	int SendNotify(int MessageID);

	//Message constants
	static const int	Message_Quit		=	100;
	static const int	Message_DisplayLogin=	101;

private:
	//Singleton
	static CUIMain	*mSingleton;

	bool		Started;
	//=============
	// Thread related
	//=============
	pthread_t	m_uithread;
	friend		void* UIThreadStarter(void* class_pointer); //Helper function to give the created thread the right class pointer
	void*		UIThread(void);
	//When other threads notify this thread the notification will be put in this vector
	std::list<int>	m_MessageQueue;
	pthread_mutex_t	m_message_mutex;

	//=============
	// Ogre related
	//=============
	Ogre::Root			*mRoot;
	Ogre::Camera		*mCamera;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RenderWindow	*mWindow;
	CInputHandler		*mInputHandler;
	// CEGUI related
	CGUIHandler			*mGUIHandler;
#ifdef OLD_CEGUI
	CEGUI::OgreCEGUIRenderer	*mGUIRenderer;
#else
	CEGUI::OgreRenderer			*mGUIRenderer;
#endif
	CEGUI::System			*mGUISystem;
	CEGUI::WindowManager	*mWindowManager;

	//This will be called every frame
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	int			SetupOgre(void);
	int			CleanupOgre(void);
};
