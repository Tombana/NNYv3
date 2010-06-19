#include "InputHandler.h"
#include "MainClient.h"

bool CInputHandler::windowClosing(Ogre::RenderWindow* rw)
{
	CMainClient::getSingleton().SendThreadMessage(Message_Quit);
	return false; //Do not close the window. The main thread will decide that
}

//Unattach OIS before window shutdown (very important under Linux)
void CInputHandler::windowClosed(Ogre::RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager ){
			if( mKeyboard ) mInputManager->destroyInputObject(mKeyboard);
			if( mMouse ) mInputManager->destroyInputObject(mMouse);
			OIS::InputManager::destroyInputSystem(mInputManager);
			mKeyboard = 0;
			mMouse = 0;
			mInputManager = 0;
		}
	}
}