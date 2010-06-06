#include "InputHandler.h"
#include "MainClient.h"

CInputHandler::CInputHandler(CWorldManager& World, Ogre::RenderWindow *window, Ogre::Camera *Cam, Ogre::SceneManager *SceneMgr, Ogre::RaySceneQuery *RaySceneQuery) :
	mWorld(World), mWindow(window), mCamera(Cam), mSceneMgr(SceneMgr), mRaySceneQuery(RaySceneQuery),
		mInputManager(0), mKeyboard(0), mMouse(0), mGUISystem(CEGUI::System::getSingletonPtr()),
		mCamNode(0), mCamDist(200), MinCamDist(100), MaxCamDist(800)
{
	size_t windowHnd = 0;
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	
	std::ostringstream windowHndStr;
	windowHndStr << windowHnd;
	
	OIS::ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
	
	mInputManager = OIS::InputManager::createInputSystem( pl );

	//Create the keyboard and mouse device in buffered mode
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);

	//Register this class as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	//The CamNode will be the point that the camera rotates around, so a point at the player
	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CameraNode");
	mCamNode->attachObject(mCamera);
	mCamera->setPosition(Ogre::Vector3(0,0,mCamDist)); //Set the camera 100 units in front the node
	mCamera->lookAt(Ogre::Vector3(0,0,0)); //Look at the node
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
	return true;
}

bool CInputHandler::keyPressed(const OIS::KeyEvent &arg)
{
	if( mGUISystem ){
		mGUISystem->injectKeyDown(arg.key);
		mGUISystem->injectChar(arg.text);
	}

	switch (arg.key){
		case OIS::KC_ESCAPE:
			CMainClient::getSingleton().SendThreadMessage(Message_Quit);
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
		mCamDist -= Ogre::Real(arg.state.Z.rel) / 15.0f;
		if( mCamDist < MinCamDist ) mCamDist = MinCamDist;
		else if( mCamDist > MaxCamDist ) mCamDist = MaxCamDist;
		mCamera->setPosition(Ogre::Vector3(0,0,mCamDist));
	}
	//Look around when right mouse button is down (regardless of wether the mouse is on a window)
	if (arg.state.buttonDown(OIS::MB_Right))
	{
		mCamNode->yaw(Ogre::Degree(-0.1 * arg.state.X.rel), Ogre::Node::TS_WORLD);
		mCamNode->pitch(Ogre::Degree(-0.1 * arg.state.Y.rel), Ogre::Node::TS_LOCAL);
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
		CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
		Ogre::Ray ray(mCamera->getCameraToViewportRay(mousePos.d_x, mousePos.d_y));
		mRaySceneQuery->setRay(ray);
		Ogre::RaySceneQueryResult& qryResult = mRaySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator it = qryResult.begin();
		if( it != qryResult.end() ){
			Ogre::Vector3 Collision = ray.getPoint(it->distance);
			Collision.y += 10; //A little above click-point
			if( mWorld.LocalPlayer ){
				mWorld.LocalPlayer->AddDestination(Collision);
			}
			mCamNode->setPosition(Collision.x, Collision.y + 20, Collision.z);
		}
	}
	return true;
}

bool CInputHandler::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if( mGUISystem ) mGUISystem->injectMouseButtonUp(convertButton(id));
	return true;
}