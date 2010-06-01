#include "ConsoleOverlay.h"

ConsoleOverlay::ConsoleOverlay() : mOverlayMgr( Ogre::OverlayManager::getSingletonPtr() )
{
	//Retrieve the overlay and panel.
	//It is already loaded by a script in the media directory
	mOverlay = mOverlayMgr->getByName( "Console/Overlay" );
	mPanel = static_cast<Ogre::BorderPanelOverlayElement*>(mOverlayMgr->getOverlayElement( "Console/Panel" ));
	mTextArea = static_cast<Ogre::TextAreaOverlayElement*>(mOverlayMgr->getOverlayElement( "Console/TextArea" ));
}

ConsoleOverlay::~ConsoleOverlay()
{
}

bool ConsoleOverlay::IsVisible(void)
{
	return mOverlay->isVisible();
}

void ConsoleOverlay::Show(bool Enable)
{
	if( Enable ) mOverlay->show();
	else mOverlay->hide();
}

void ConsoleOverlay::setText(char *szString)
{
	mTextArea->setCaption(szString);
}

void ConsoleOverlay::setText(Ogre::String szString) // now You can use Ogre::String as text
{
	mTextArea->setCaption(szString);
}
