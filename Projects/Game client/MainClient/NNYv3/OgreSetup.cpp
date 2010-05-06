#include "UIMain.h"
#include "InputHandler.h"
#include "OgreConfigFile.h"


// This function will set up everything required by Ogre
// and it will ask the user for display settings
// At the end of this function Ogre is ready to render.
// This function is mostly taken from tutorials and sample programs.
int CUIMain::SetupOgre(void)
{
	//=================
	//Create the Ogre root object
	// It's possible to specify as parameters the paths to the:
	// plugin file (what render systems it has to load), config file (screen resolution etc) and log file
	//=================
	if( !mRoot ) mRoot = OGRE_NEW Ogre::Root();

	//=================
	// Tell Ogre where all the needed resources are (rendersystems and so on)
	//=================
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
	
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements()){
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	//=================
	// Set up the render system.
	// Ogre will ask the user for display settings
	//=================
	if( !mRoot->showConfigDialog() )
		return 0; //The user probably clicked cancel

	mWindow = mRoot->initialise(true, "NNYv3");

	//=================
	// Load all the resources. For now, just load all resources at once. The following is from a tutorial:
	// In a very large game or application, we may have hundreds or even thousands of resources
	// that our game uses - everything from meshes to textures to scripts. At any given time though,
	// we probably will only be using a small subset of these resources. To keep down memory requirements,
	// we can load only the resources that our application is using. We do this by dividing the resources
	// into sections and only initializing them as we go. 
	//=================
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//=================
	// Preparing the scene
	//=================
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC); //ST_EXTERIOR_CLOSE if we want to render terrain
	
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setNearClipDistance(5);
	mCamera->setPosition(0,0,200); //set the camera 200 units in front of the screen
	
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.9,0.9,0.9));
	
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	
	//Set a moving cloud texture as background
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);

	//=================
	// Set up the CEGUI system
	//=================
	try{
#ifdef OLD_CEGUI
		mGUIRenderer = new CEGUI::OgreCEGUIRenderer(mWindow, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
		mGUISystem = new CEGUI::System(mGUIRenderer);
#else
		mGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
		mGUISystem = CEGUI::System::getSingletonPtr();
#endif
		
		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

#ifdef OLD_CEGUI
		CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
		mGUISystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
		CEGUI::MouseCursor::getSingleton().setImage(mGUISystem->getDefaultMouseCursor());
		mGUISystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
#else
		CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
		mGUISystem->setDefaultMouseCursor("TaharezLook", "MouseArrow");
		CEGUI::MouseCursor::getSingleton().setImage(mGUISystem->getDefaultMouseCursor());
		mGUISystem->setDefaultFont("BlueHighway-12");
#endif

		mWindowManager = CEGUI::WindowManager::getSingletonPtr();
		mGUIHandler = new CGUIHandler(mWindowManager);
		mGUIHandler->Setup();

	}catch( CEGUI::Exception& e ){
		std::cerr << "[ERROR] Exception in CEGUI:\n" << e.getMessage() << std::endl;
	}

	//=================
	// Create the input handler
	//=================
	mInputHandler = new CInputHandler(mWindow, mGUISystem);
	mRoot->addFrameListener(mInputHandler);
	mRoot->addFrameListener(this);

	return 1;
}

int CUIMain::CleanupOgre(void)
{
	//Clean up the input system
	if( mRoot ){
		mRoot->removeFrameListener(this);
		if( mInputHandler) mRoot->removeFrameListener(mInputHandler);
	}
	if(mInputHandler) delete mInputHandler;
	mInputHandler = 0;

	//Clean up CEGUI
	if( mGUIHandler ) delete mGUIHandler;
	mGUIHandler = 0;
#ifdef OLD_CEGUI
	if(mGUISystem) delete mGUISystem;
	if(mGUIRenderer) delete mGUIRenderer;
#else
	CEGUI::OgreRenderer::destroySystem();
#endif
	mGUISystem = 0;
	mGUIRenderer = 0;

	//Clean up the main Ogre system
	//Deleting the root object will also clean up all other created classes
	if(mRoot) OGRE_DELETE mRoot;
	mRoot = 0;

	return 1;
}