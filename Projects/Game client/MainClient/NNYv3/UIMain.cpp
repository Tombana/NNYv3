#include "UIMain.h"
#include "MainClient.h"
#include "GUIHandler.h"
#include <iostream>

CUIMain* CUIMain::mSingleton = 0;

CUIMain::CUIMain(void) :
	Started(false), m_uithread(), m_MessageQueue(), m_message_mutex(PTHREAD_MUTEX_INITIALIZER),
	mRoot(0), mCamera(0), mSceneMgr(0), mWindow(0),
	mInputHandler(0), mGUIHandler(), mGUIRenderer(0), mGUISystem(0), mWindowManager(0), mRootWindow(0)
{
	if( mSingleton == 0 ) mSingleton = this;
}

CUIMain::~CUIMain(void)
{
	CleanupOgre();
	if( mSingleton == this ) mSingleton = 0;
}

int CUIMain::StartUI(void)
{
	if( Started ) return 1;
	Started = true;

	//Note: I first wanted to set up Ogre here so if there
	//were any errors then I could return 0 so that
	//the main thread would not continue.
	//However: Everything with Ogre should be done from the
	//same thread, so thats why the setup also happens in the seperate thread.

	//Create the ui thread
	if( pthread_create(&m_uithread, NULL, UIThreadStarter, this) ){
		std::cout << "[ERROR] pthread: pthread_create() failed!" << std::endl;
		return 0;
	}

	return 1;
}

int CUIMain::WaitForExit(void)
{
	pthread_join(m_uithread, NULL);
	return 1;
}

int CUIMain::SendNotify(int MessageID)
{
	if( MessageID ){
		pthread_mutex_lock(&m_message_mutex);
		m_MessageQueue.push_back(MessageID);
		pthread_mutex_unlock(&m_message_mutex);
	}
	return 0;
}


void* UIThreadStarter(void* class_pointer){
	return ((CUIMain*)class_pointer)->UIThread();
}

void* CUIMain::UIThread(void)
{
	//Setup Ogre
	int Success = 0;
	try{
		Success = SetupOgre();
	}catch( Ogre::Exception& e ) {
		std::cerr << "[ERROR] An exception has occurred while loading Ogre: \n" << e.what() << std::endl;
    }
	if( !Success ){
		Started = false;
		return 0;
	}
	//Show the loading screen
	//DisplayLoadingScreen();

	//Setup physics

	//Setup sound

	//Start the rendering loop
	mRoot->startRendering();
	//The rendering has ended: the user quit the window or an error occured

	//Clean up
	CleanupOgre();

	std::cout << "------------------GUI THREAD DONE!\n";

	return 0;
}

bool CUIMain::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ContinueRendering = true;
	if( m_MessageQueue.size() ){ //If there are messages from other threads to process
		pthread_mutex_lock(&m_message_mutex);
		int Message = m_MessageQueue.front(); //Get the message from the queue
		m_MessageQueue.pop_front(); //Remove the message from the queue
		pthread_mutex_unlock(&m_message_mutex);
		switch(Message){
			case Message_Quit:
				ContinueRendering = false;
				break;
			case Message_DisplayLogin:
				DisplayLoginScreen();
				break;
			default:
				break;
		}
	}
	return ContinueRendering;
}

int	CUIMain::DisplayLoginScreen(void)
{
	//Load the window layout from file
	CEGUI::Window *LoginWindow = mWindowManager->loadWindowLayout("loginscreen.layout");
	//Subscribe the handler
	CEGUI::Window *LoginButton = mWindowManager->getWindow("LoginWindow/ButtonLogin");
	LoginButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::LoginBtnClick, &mGUIHandler));
	mRootWindow->addChildWindow(LoginWindow);
	return 1;
}