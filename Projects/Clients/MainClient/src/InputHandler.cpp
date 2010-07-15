#include "InputHandler.h"
#include "MainClient.h"
#include "WorldManager.h"
#include "Camera.h"
#include "ThreadMessagesConstants.h"

CInputHandler::CInputHandler(CWorldManager& World, CCamera& Camera, Ogre::RenderWindow *window, Ogre::SceneManager *SceneMgr) :
	mWorld(World), mCamera(Camera), mWindow(window), mSceneMgr(SceneMgr),
		mInputManager(0), mKeyboard(0), mMouse(0), mGUISystem(CEGUI::System::getSingletonPtr()),
		mEscapeTimer(0), mMaxEscapeDelay(1.5)
{
	size_t windowHnd = 0;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	mInputManager = OIS::InputManager::createInputSystem( windowHnd );

	//Create the keyboard and mouse device in buffered mode
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	//Register this class as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
}

CInputHandler::~CInputHandler(void)
{
	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	//This function destroys the input objects
	windowClosed(mWindow);
}

bool CInputHandler::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mKeyboard->capture();
	mMouse->capture();
	mEscapeTimer += evt.timeSinceLastFrame;
	return true;
}

bool CInputHandler::keyPressed(const OIS::KeyEvent &arg)
{
	if( mGUISystem ){
		bool HandledKeyDown = mGUISystem->injectKeyDown(arg.key);
		bool HandledChar = mGUISystem->injectChar(arg.text);
		if( HandledKeyDown || HandledChar ) return true;
	}

	switch (arg.key){
		case OIS::KC_ESCAPE:
			//If an entity was selected -> deselect
			if( !CUIMain::getSingleton().DeselectEntity() ){
				//If nothing was selected
				//Quit after pressing esc two times within the delay time
				if( mEscapeTimer < mMaxEscapeDelay ){
					CMainClient::getSingleton().SendThreadMessage(Message_Quit);
				}
				mEscapeTimer = 0;
			}
            break;
		case OIS::KC_GRAVE:
			CUIMain::getSingleton().ToggleConsole();
			break;
		default:
			break;
	}

	return true;
}

bool CInputHandler::keyReleased(const OIS::KeyEvent &arg)
{
	if( mGUISystem ) mGUISystem->injectKeyUp(arg.key);
	return true;
}

bool CInputHandler::mouseMoved(const OIS::MouseEvent &arg)
{
	if( mGUISystem ){
		mGUISystem->injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
		// Scroll wheel.
		if (arg.state.Z.rel) mGUISystem->injectMouseWheelChange(arg.state.Z.rel / 120.0f);
	}
	//Zoom using scroll wheel
	if( arg.state.Z.rel ){
		mCamera.mCamZoomSpeed -= 1.2 * Ogre::Real(arg.state.Z.rel);
	}
	//Look around when right mouse button is down (regardless of wether the mouse is on a window)
	if (arg.state.buttonDown(OIS::MB_Right))
	{
		mCamera.mCamYawSpeed -= 1.2 * Ogre::Real(arg.state.X.rel);
		mCamera.mCamPitchSpeed -= 1.2 * Ogre::Real(arg.state.Y.rel);
	}
	return true;
}

//CEGUI and OIS do not use the same constants for mouse buttons so we have to convert them
CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
		case OIS::MB_Left:		return CEGUI::LeftButton;
		case OIS::MB_Right:		return CEGUI::RightButton;
		case OIS::MB_Middle:	return CEGUI::MiddleButton;
		case OIS::MB_Button3:	return CEGUI::X1Button;
		default:				return CEGUI::LeftButton;
    }
}

bool CInputHandler::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	bool ProcessedByGUI = false;
	if( mGUISystem ) ProcessedByGUI = mGUISystem->injectMouseButtonDown(convertButton(id));
	if( ProcessedByGUI ) return true;

	if( id == OIS::MB_Left ){
		return CUIMain::getSingleton().LeftClickInWorld();
	}
	return true;
}

bool CInputHandler::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if( mGUISystem ) mGUISystem->injectMouseButtonUp(convertButton(id));
	return true;
}