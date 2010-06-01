#pragma once

#include <Ogre.h>
#define OIS_DYNAMIC_LIB //OIS is the input system
#include <OIS/OIS.h>

#include <CEGUI/CEGUI.h>

class CInputHandler : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	CInputHandler(Ogre::RenderWindow *window, Ogre::Camera *Cam, Ogre::SceneManager *SceneMgr, Ogre::RaySceneQuery *RaySceneQuery);
	~CInputHandler(void);
	
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    // KeyListener
    virtual bool keyPressed(const OIS::KeyEvent &arg);
    virtual bool keyReleased(const OIS::KeyEvent &arg);

    // MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent &arg);
    virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) ;

	//WindowListener
	bool windowClosing(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);

private:
	Ogre::RenderWindow	*mWindow;
	Ogre::Camera		*mCamera;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RaySceneQuery *mRaySceneQuery;
	OIS::InputManager	*mInputManager;
	OIS::Keyboard		*mKeyboard;
	OIS::Mouse			*mMouse;
	CEGUI::System		*mGUISystem;

	//Camera related
	Ogre::SceneNode		*mCamNode;
	Ogre::Real			mCamDist; //Zoom. Higher means further away
	Ogre::Real			MinCamDist;
	Ogre::Real			MaxCamDist;
};
