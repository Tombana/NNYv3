#include "GUIHandler.h"
#include "MainClient.h"
#include "ThreadMessagesConstants.h"

CGUIHandler::CGUIHandler(Ogre::RenderWindow *Window, Ogre::SceneManager *SceneMgr) :
	mGUIRenderer(0), mGUISystem(0), mWindowManager(0), mRootWindow(0),
		DefaultButtonSize(CEGUI::UDim(0, DefaultButtonWidth), CEGUI::UDim(0, DefaultButtonHeight)), WaitScreen(0), WaitScreenLabel(0)
{
	try{
#ifdef OLD_CEGUI
		mGUIRenderer = new CEGUI::OgreCEGUIRenderer(Window, Ogre::RENDER_QUEUE_OVERLAY, false, 3000, SceneMgr);
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

		//All windows have a parent/child structure.
		//Its common to name windows as follows: NNYv3/TabCtrl/Page2/ObjectTypeList

		//Create the root window (fullscreen)
		mRootWindow = mWindowManager->createWindow("DefaultWindow", "NNYv3");

		//Create the quit button
		CEGUI::Window *btnQuit = mWindowManager->createWindow("TaharezLook/Button", "NNYv3/QuitButton");
		btnQuit->setText("Quit");
		//CEGUI::UDim - Taken from tutorial:
		//When setting the size you must create a UDim object to tell it what size it should be.
		//The first parameter is the relative size of the object in relation to its parent.
		//The second parameter is the absolute size of the object (in pixels).
		//The important thing to realize is that usually you are only supposed to set one of the two parameters to UDim.
		//The other parameter must then be 0.
		//If you want something to be 100 px from the right side of the screen, you would set the relative
		//parameter to 1 (completely right) and the absolute parameter to -100.
		btnQuit->setSize(DefaultButtonSize);
		btnQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(1, -DefaultButtonWidth), CEGUI::UDim(0,0)));

		//Set the function that handles the quit button
		btnQuit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::QuitBtnClick, this));

		mRootWindow->addChildWindow(btnQuit);

		CEGUI::System::getSingleton().setGUISheet(mRootWindow);

		LoadWaitScreen();

	}catch( CEGUI::Exception& e ){
		std::cerr << "[ERROR] Exception in CEGUI:\n" << e.getMessage() << std::endl;
	}
}

CGUIHandler::~CGUIHandler(void)
{
	if( mWindowManager ) mWindowManager->destroyAllWindows();
	mRootWindow = 0;
	mWindowManager = 0;
#ifdef OLD_CEGUI
	if(mGUISystem) delete mGUISystem;
	if(mGUIRenderer) delete mGUIRenderer;
#else
	CEGUI::OgreRenderer::destroySystem();
#endif
	mGUISystem = 0;
	mGUIRenderer = 0;
}

bool CGUIHandler::QuitBtnClick(const CEGUI::EventArgs &e)
{
	CMainClient::getSingleton().SendThreadMessage(Message_Quit);
	return true;
}

//================
// Message boxes
//================

//Message box handlers in CEGUI format.
bool CGUIHandler::MessageBoxBtnOkClick(const CEGUI::EventArgs &e)
{
	return MessageBoxClick(e, MsgBoxBtnOk);
}

bool CGUIHandler::MessageBoxBtnYesClick(const CEGUI::EventArgs &e)
{
	return MessageBoxClick(e, MsgBoxBtnYes);
}

bool CGUIHandler::MessageBoxBtnNoClick(const CEGUI::EventArgs &e)
{
	return MessageBoxClick(e, MsgBoxBtnNo);
}

//The message box handler calls the callback function if one is specified
bool CGUIHandler::MessageBoxClick(const CEGUI::EventArgs &e, int Button)
{
	const CEGUI::WindowEventArgs& arg = static_cast<const CEGUI::WindowEventArgs&>(e);
	CEGUI::Window *MsgBox = arg.window->getParent();
	CallbackFunction* callback = (CallbackFunction*)MsgBox->getUserData();
	bool Destroy = true;
	if( callback ) Destroy = callback->CallCallback((void*)Button);
	if( Destroy ){
		if( callback ) delete callback;
		mWindowManager->destroyWindow(MsgBox);
	}
	return true;
}

//Creates and shows a message box
CEGUI::Window* CGUIHandler::MsgBox(std::string Text, std::string Title, int Buttons, CallbackFunction* Callback, std::string WindowName)
{
	CEGUI::Window *MsgBox = 0;
	try{
		MsgBox = mWindowManager->createWindow("TaharezLook/FrameWindow", WindowName);
	}catch( CEGUI::AlreadyExistsException ){
		return 0;
	}
	MsgBox->setUserData(Callback);

	CEGUI::Window *Label = mWindowManager->createWindow("TaharezLook/StaticText");

	Label->setProperty("HorzFormatting", "WordWrapLeftAligned");
	Label->setArea(CEGUI::URect(CEGUI::UDim(0.0,10), CEGUI::UDim(0,28), CEGUI::UDim(1,-10), CEGUI::UDim(1,-40)));
	
	MsgBox->setSize(CEGUI::UVector2(CEGUI::UDim(0,360), CEGUI::UDim(0,160)));
	MsgBox->setMinSize(MsgBox->getSize());
	MsgBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,-180), CEGUI::UDim(0.5,-130)));

	MsgBox->setText(Title);
	Label->setText(Text);

	MsgBox->addChildWindow(Label);
	MsgBox->setAlwaysOnTop(true);
	mRootWindow->addChildWindow(MsgBox);

	if( Buttons == MsgBoxBtnsYesNo ){
		CEGUI::Window *ButtonYes = mWindowManager->createWindow("TaharezLook/Button");
		CEGUI::Window *ButtonNo = mWindowManager->createWindow("TaharezLook/Button");
		ButtonYes->setSize(DefaultButtonSize);
		ButtonYes->setPosition(CEGUI::UVector2(CEGUI::UDim(0,50), CEGUI::UDim(1,-35)));
		ButtonNo->setSize(DefaultButtonSize);
		ButtonNo->setPosition(CEGUI::UVector2(CEGUI::UDim(1,-150), CEGUI::UDim(1,-35)));
		ButtonYes->setText("Yes");
		ButtonNo->setText("No");
		ButtonYes->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::MessageBoxBtnYesClick, this));
		ButtonNo->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::MessageBoxBtnNoClick, this));
		MsgBox->addChildWindow(ButtonYes);
		MsgBox->addChildWindow(ButtonNo);
	}else{
		CEGUI::Window *ButtonOk = mWindowManager->createWindow("TaharezLook/Button");
		ButtonOk->setSize(DefaultButtonSize);
		ButtonOk->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,-(DefaultButtonWidth/2)), CEGUI::UDim(1,-35)));
		ButtonOk->setText("Ok");
		ButtonOk->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::MessageBoxBtnOkClick, this));
		MsgBox->addChildWindow(ButtonOk);
	}
	return MsgBox;
}

int	CGUIHandler::LoadWaitScreen(void)
{
	WaitScreen = mWindowManager->createWindow("TaharezLook/FrameWindow");
	WaitScreenLabel = mWindowManager->createWindow("TaharezLook/StaticText");

	WaitScreenLabel->setProperty("HorzFormatting", "WordWrapLeftAligned");
	WaitScreenLabel->setArea(CEGUI::URect(CEGUI::UDim(0.0,10), CEGUI::UDim(0,28), CEGUI::UDim(1,-10), CEGUI::UDim(1,-40)));
	WaitScreen->setSize(CEGUI::UVector2(CEGUI::UDim(0,360), CEGUI::UDim(0,160)));
	WaitScreen->setMinSize(WaitScreen->getSize());
	WaitScreen->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5,-180), CEGUI::UDim(0.5,-130)));

	WaitScreen->setText("Please wait");
	WaitScreenLabel->setText("Please wait");

	WaitScreen->addChildWindow(WaitScreenLabel);

	return 1;
}

int CGUIHandler::DisplayWaitScreen(std::string Text)
{
	if( WaitScreenLabel ) WaitScreenLabel->setText(Text);
	if( WaitScreen ){
		//The user might have moved the waitscreen, so reset it to the middle
		WaitScreen->setSize(CEGUI::UVector2(CEGUI::UDim(0.40,0), CEGUI::UDim(0.25,0)));
		//Attach it to the root window so it is drawn
		mRootWindow->addChildWindow(WaitScreen);
		//Bring it to the front
		WaitScreen->activate();
	}
	return 1;
}

int CGUIHandler::CloseWaitScreen(void)
{
	if( WaitScreen ){
		//Detach it from parent so it is not drawn
		CEGUI::Window* Parent = WaitScreen->getParent();
		if( Parent ) Parent->removeChildWindow(WaitScreen);
	}
	return 1;
}

//===================
// Login section
//===================
int CGUIHandler::DisplayWorldSelect(const std::vector<WORLDSERVER>& Servers)
{
	if( !DisplayWorldSelect(true) ){
		try{
			//Load the window layout from file
			CEGUI::Window *WorldSelectWindow = mWindowManager->loadWindowLayout("worldselect.layout");
			//WorldSelectWindow->setMinSize( CEGUI::UVector2( CEGUI::UDim(0, 400), CEGUI::UDim(0, 400) ) );
			//Subscribe the handler for the login and quit button
			CEGUI::Window *LoginButton = mWindowManager->getWindow("WorldSelectWindow/ButtonGo");
			LoginButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::WorldSelectBtnClick, this));
			CEGUI::Window *QuitButton = mWindowManager->getWindow("WorldSelectWindow/ButtonQuit");
			QuitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::QuitBtnClick, this));
			//Attach the window to the root window so it's visible
			mRootWindow->addChildWindow(WorldSelectWindow);
		}catch(CEGUI::UnknownObjectException){}
	}
	
	//Add the world servers to the list
	try{
		CEGUI::Listbox* ListServers = static_cast<CEGUI::Listbox*>(mWindowManager->getWindow("WorldSelectWindow/ListWorlds"));
		ListServers->resetList();
		for( std::vector<WORLDSERVER>::const_iterator server = Servers.begin(); server != Servers.end(); ++server ){
			CEGUI::String Text;
			Text.append("[");
			Text.append( server->Online ? "x" : " " );
			Text.append( "] " );
			Text.append( server->Name );
			CEGUI::ListboxTextItem *listboxitem = new CEGUI::ListboxTextItem(Text);
			listboxitem->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
			listboxitem->setSelected(ListServers->getItemCount() == 0);
			ListServers->addItem(listboxitem);
		}
	}catch(CEGUI::UnknownObjectException){}

	return 1;
}

int CGUIHandler::DisplayWorldSelect(bool Enabled)
{
	try{ //See if the window already exists
		CEGUI::Window *WorldSelectWindow = mWindowManager->getWindow("WorldSelectWindow");
		WorldSelectWindow->setEnabled(Enabled);
	}catch(CEGUI::UnknownObjectException){ return 0; };
	return 1;
}

int CGUIHandler::CloseWorldSelect(void)
{
	mWindowManager->destroyWindow("WorldSelectWindow");
	return 1;
}

int	CGUIHandler::DisplayLoginScreen(const std::string& RememberedUsername)
{
	bool CreateNew = false;
	try{ //See if the window already exists
		CEGUI::Window *LoginWindow = mWindowManager->getWindow("LoginWindow");
		LoginWindow->setEnabled(true);
	}catch(CEGUI::UnknownObjectException){ CreateNew = true; };
	if( CreateNew ){
		try{
			//Load the window layout from file
			CEGUI::Window *LoginWindow = mWindowManager->loadWindowLayout("loginscreen.layout");
			LoginWindow->setMinSize( CEGUI::UVector2( CEGUI::UDim(0, 200), CEGUI::UDim(0, 300) ) );
			//Set the password box to be masked
			CEGUI::Editbox *Password = static_cast<CEGUI::Editbox*>(mWindowManager->getWindow("LoginWindow/Password"));
			Password->setMaskCodePoint('*');
			Password->setTextMasked(true);
			//Subscribe the handler for the login and about button
			CEGUI::Window *LoginButton = mWindowManager->getWindow("LoginWindow/ButtonLogin");
			LoginButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::LoginBtnClick, this));
			CEGUI::Window *AboutButton = mWindowManager->getWindow("LoginWindow/ButtonAbout");
			AboutButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&CGUIHandler::AboutBtnClick, this));
			//Attach the window to the root window so it's visible
			mRootWindow->addChildWindow(LoginWindow);
		}catch(CEGUI::UnknownObjectException){}
	}
	
	//Set the remembered username
	if( !RememberedUsername.empty() ){
		try{
			CEGUI::Window *txtUsername = mWindowManager->getWindow("LoginWindow/Username");
			txtUsername->setText(RememberedUsername);
		}catch(CEGUI::UnknownObjectException){}
	}
	return 1;
}

int CGUIHandler::CloseLoginScreen(void)
{
	mWindowManager->destroyWindow("LoginWindow");
	return 1;
}

bool CGUIHandler::WorldSelectBtnClick(const CEGUI::EventArgs &e)
{
	int ServerIndex = -1;
	try{
		CEGUI::Listbox* ListServers = static_cast<CEGUI::Listbox*>(mWindowManager->getWindow("WorldSelectWindow/ListWorlds"));
		CEGUI::ListboxItem* Item = ListServers->getFirstSelectedItem();
		if( Item ) ServerIndex = ListServers->getItemIndex(Item);
	}catch(CEGUI::UnknownObjectException){}catch(CEGUI::InvalidRequestException){}
	if( ServerIndex == -1 ){
		MsgBox("Please select a server!", "Error");
	}else{
		DisplayWorldSelect(false);
		CMainClient::getSingleton().SendThreadMessage( new CMessageParamsConnectWorld(ServerIndex) );
	}
	return true;
}

bool CGUIHandler::LoginBtnClick(const CEGUI::EventArgs &e)
{
	std::string Username, Password;
	try{
		CEGUI::Window *txtUsername = mWindowManager->getWindow("LoginWindow/Username");
		Username = txtUsername->getText().c_str();
	}catch(CEGUI::UnknownObjectException){}
	try{
		CEGUI::Window *txtPassword = mWindowManager->getWindow("LoginWindow/Password");
		Password = txtPassword->getText().c_str();
	}catch(CEGUI::UnknownObjectException){}

	if( Username.empty() || Password.empty() )
		MsgBox("Please fill in both the username and password!", "Error");
	else{
		try{ //Disable the login window
			CEGUI::Window *LoginWindow = mWindowManager->getWindow("LoginWindow");
			LoginWindow->setEnabled(false);
		}catch(CEGUI::UnknownObjectException){}
		//Send login message
		CMainClient::getSingleton().SendThreadMessage(new CMessageParamsLogin(Username, Password));
	}
	return true;
}

bool CGUIHandler::AboutBtnClick(const CEGUI::EventArgs &e)
{
	MsgBox("NNYv3 stands for No Name Yet version 3.\nNNYv3 is an open source MMORPG client made mainly by nitrix and Tombana.", "About NNYv3", MsgBoxBtnsOk, 0, "NNYv3/Aboutbox");
	return true;
}