//Tombana: taken from http://www.ogre3d.org/wiki/index.php/Simple_text
//I added some comments to the code and did some changes, added some functions.
//Added Ogre:: so it does not have to do 'using namespace Ogre'
//It had a few bugs which I fixed.

//-----------------------------------------------------------------------------
// Lonewolff
//
// Filename:    OgreText.h
// Description: Class for simple text in Ogre (Version 040507:18.30)
//-----------------------------------------------------------------------------

#ifndef __OgreText_H__
#define __OgreText_H__

//Tombana: I included the complete Ogre.h instead of the 2 seperate files below because else
//it would not allow the static_cast's in here.
#include <Ogre.h>
//#include "OgreOverlayManager.h"
//#include "OgreStringConverter.h"
#include "OgreTextAreaOverlayElement.h"

class OgreText
{
public:
	OgreText()
	{
		olm = Ogre::OverlayManager::getSingletonPtr();
		//This is only done once, not every time the class is created
		//It creates a new 'overlay' and a 'panel'
		if( !refcount ){
			overlay = olm->create("GUI_OVERLAY");

			panel = static_cast<Ogre::OverlayContainer*>(olm->createOverlayElement("Panel","TEXTPANEL"));
			panel->setMetricsMode(Ogre::GMM_PIXELS);
			panel->setPosition(0,0);
			panel->setDimensions(1.0f,1.0f);

			overlay->add2D(panel);
		}
		++(this->refcount);
		//Retrieve the overlay and panel
		overlay=olm->getByName("GUI_OVERLAY");
		panel=static_cast<Ogre::OverlayContainer*>(olm->getOverlayElement("TEXTPANEL"));

		//Create an element for every instance of this class
		elementnum = totalelementnum;
		szElement = "element_" + Ogre::StringConverter::toString(elementnum);
		++totalelementnum;

		//Create the TextArea
		textArea = static_cast<Ogre::TextAreaOverlayElement*>(olm->createOverlayElement("TextArea",szElement));
		panel->addChild(textArea);
		overlay->show();
	}
	~OgreText()
	{
		szElement = "element_" + Ogre::StringConverter::toString(elementnum);
		olm->destroyOverlayElement(szElement);
		--(this->refcount);
		if( refcount == 0 ){
			olm->destroyOverlayElement("TEXTPANEL");
			olm->destroy("GUI_OVERLAY");
		}
	}
	void Show(bool Enable)
	{
		if( Enable ) textArea->show();
		else textArea->hide();
	}
	void setText(char *szString)
	{
		textArea->setCaption(szString);
		textArea->setDimensions(1.0f,1.0f);
		textArea->setMetricsMode(Ogre::GMM_RELATIVE);
		textArea->setFontName("BlueHighway");
		textArea->setCharHeight(0.03f);
	}
	void setText(Ogre::String szString) // now You can use Ogre::String as text
	{
		textArea->setCaption(szString);
		textArea->setDimensions(1.0f,1.0f);
		textArea->setMetricsMode(Ogre::GMM_RELATIVE);
		textArea->setFontName("BlueHighway");
		textArea->setCharHeight(0.03f);
	}
	void setPos(float x,float y)
	{
		textArea->setPosition(x,y);
	}
	void setCol( Ogre::ColourValue colour )
	{
		textArea->setColour(colour);
	}
private:
	static int refcount;
	Ogre::OverlayManager	*olm;
	Ogre::Overlay			*overlay;
	Ogre::OverlayContainer	*panel;
	Ogre::TextAreaOverlayElement *textArea;
	static int totalelementnum;
	int elementnum;
	Ogre::String szElement;
};


#endif
