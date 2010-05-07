#pragma once

#include <Ogre.h>
#define OIS_DYNAMIC_LIB //OIS is the input system
#include <OIS/OIS.h>

#include <CEGUI/CEGUI.h>

class CInputHandler : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	CInputHandler(Ogre::RenderWindow *window);
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
	OIS::InputManager	*mInputManager;
	OIS::Keyboard		*mKeyboard;
	OIS::Mouse			*mMouse;
	CEGUI::System		*mGUISystem;
};
