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
	//Ogre defaultly logs to console
	// To prevent this the LogManager has to be created
	// before the Root object.
	//=================
	Ogre::LogManager* logMgr = OGRE_NEW Ogre::LogManager;
	logMgr->createLog("Ogre.log", true, false, false);

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
	mSceneMgr = mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE); //ST_EXTERIOR_CLOSE allows rendering terrain

	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setNearClipDistance(5);
	
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.9,0.9,0.9));
	//Fog will not work with sky ;)
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(0.9,0.9,0.9), 0.0, 50, 500);

	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
	
	//Set a moving cloud texture as background
	mSceneMgr->setSkyDome(true, "CloudySky", 5, 8);

	//Get a RaySceneQuery object. A SceneQuery object is a class that can query all
	//objects in a region or scene. RaySceneQuery has it as a base class. RaySceneQuery
	//can get all objects that intersect a ray.
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());

	LoadWorld();

	//=================
	// Set up the CEGUI system
	//=================
	mGUIHandler = new CGUIHandler(mWindow, mSceneMgr);

	//=================
	// Create the input handler
	//=================
	mInputHandler = new CInputHandler(mWindow, mCamera, mSceneMgr, mRaySceneQuery);
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

	//Clean up the main Ogre system
	//Deleting the root object will also clean up all other created classes
	if(mRoot) OGRE_DELETE mRoot;
	mRoot = 0;

	return 1;
}

int CUIMain::LoadWorld(void)
{
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, -10); //10 under the ground
	Ogre::MeshManager::getSingleton().createPlane("ground", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		2000,2000,20,20,true,1,5,5,Ogre::Vector3::UNIT_Z);
	Ogre::Entity *GroundEnt = mSceneMgr->createEntity("GroundEntity", "ground");
	GroundEnt->setMaterialName("Rockwall");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(GroundEnt);

	Ogre::Entity *head = mSceneMgr->createEntity("OgreHead", "ogrehead.mesh");
	Ogre::SceneNode *headnode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LocalPlayerNode", Ogre::Vector3(0,10,0));
	headnode->attachObject(head);
	return 1;
}