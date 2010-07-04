#pragma once

#include "Thread.h"
#include "ThreadMessages.h"
#include "InputHandler.h"
#include "GUIHandler.h"
#include "Camera.h"
#include <Ogre.h>
#include "ConsoleOverlay.h"
#include "WorldManager.h"

//No idea in what header I should put this: 
#define QUERY_MASK_MOUSE_MOVEMENT		0x01 //Wether this entity will intersect the mouse click ray for movement. Disable this flag for almost all entities except world fragments
#define QUERY_MASK_MOUSE_SELECTING		0x02 //Wether this entity will intersect the mouse click ray for targeting entities.

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
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RenderWindow	*mWindow;
	CInputHandler		*mInputHandler;
	// CEGUI related
	CGUIHandler			*mGUIHandler;
	//Camera related
	CCamera				mCamera;

	Ogre::RaySceneQuery	*mQueryMouseMovement;
	Ogre::RaySceneQuery *mQueryMouseSelection;

	//This will be called every frame
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	int SetupOgre(void);
	int CleanupOgre(void);
	int LoadWorld(void);

	//Console overlay
	bool				mShowConsole;
	ConsoleOverlay		*mConsoleOverlay;

	//3D mouse point indicator
	Ogre::SceneNode* mMouseIndicator;

	//=============
	// Login section
	//=============
	bool MsgBoxKickCallback(void* Param);

	//================
	// InGame related
	//================
	CWorldManager mWorld;

	int EntityNameCounter; //To generate unique entity names

	CPlayer* CreateNewPlayer(unsigned int Identifier, const CharacterInfo& characterinfo);
	
	//This will attach the 3D models to the scenenode node of a player or other entity
	//based on the looks-related-info in CharacterInfo.
	//It will also put the animation info in the CEntity class
	void AttachMeshes(CEntity* Entity, const CharacterInfo& characterinfo);
};
