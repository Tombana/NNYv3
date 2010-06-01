//Tombana: partly based on http://www.ogre3d.org/wiki/index.php/Simple_text but only for a tiny part
#ifndef __ConsoleOverlay_H__
#define __ConsoleOverlay_H__

#include <Ogre.h>
#include "OgreTextAreaOverlayElement.h"
#include "OgreBorderPanelOverlayElement.h"

//======== See Log.h for more comments regarding log/console output ==========

//TODO: Optimize: currently it re-sets the text every frame.
//When console log is long then that is a lot of useless copying.

//TODO: different colours per line. (std::cerr will be red and std::cout will be normal white/black)
//Probably hard to do since the TextArea overlay only allows one colour for all the text it holds.

//TODO: Scrolling through console

class ConsoleOverlay
{
public:
	ConsoleOverlay();
	~ConsoleOverlay();
	bool IsVisible(void);
	void Show(bool Enable);
	void setText(char *szString);
	void setText(Ogre::String szString); // now you can use Ogre::String as text
private:
	Ogre::OverlayManager			*mOverlayMgr;
	Ogre::Overlay					*mOverlay;
	Ogre::BorderPanelOverlayElement	*mPanel;
	Ogre::TextAreaOverlayElement	*mTextArea;
};

#endif