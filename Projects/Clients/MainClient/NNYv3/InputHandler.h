#pragma once

#include <Ogre.h>
#define OIS_DYNAMIC_LIB //OIS is the input system
#include <OIS/OIS.h>

#include <CEGUI/CEGUI.h>

class CWorldManager;
class CCamera;

class CInputHandler : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	CInputHandler(CWorldManager& World, CCamera& Camera, Ogre::RenderWindow *window, Ogre::SceneManager *SceneMgr, Ogre::RaySceneQuery *RaySceneQuery);
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
	CWorldManager&		mWorld;
	CCamera&			mCamera;
	Ogre::RenderWindow	*mWindow;
	Ogre::SceneManager	*mSceneMgr;
	Ogre::RaySceneQuery *mRaySceneQuery;
	OIS::InputManager	*mInputManager;
	OIS::Keyboard		*mKeyboard;
	OIS::Mouse			*mMouse;
	CEGUI::System		*mGUISystem;
};
