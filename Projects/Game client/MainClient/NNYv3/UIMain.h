#pragma once

#include "pthread.h"
#include "InputHandler.h"
#include "GUIHandler.h"
#include <Ogre.h>


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

	//This will be called every frame
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	int SetupOgre(void);
	int CleanupOgre(void);
};
