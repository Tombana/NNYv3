#include "UIMain.h"
#include "MainClient.h"
#include "GUIHandler.h"
#include <iostream>

CUIMain* CUIMain::mSingleton = 0;

CUIMain::CUIMain(void) : Ogre::FrameListener(), CThreadMessages(),
	Started(false), m_uithread(),
	mRoot(0), mCamera(0), mSceneMgr(0), mWindow(0),
	mInputHandler(0), mGUIHandler(0)
{
	if( mSingleton == 0 ) mSingleton = this;
}

CUIMain::~CUIMain(void)
{
	CleanupOgre();
	if( mSingleton == this ) mSingleton = 0;
}

int CUIMain::StartUI(void)
{
	if( Started ) return 1;
	Started = true;

	//Note: I first wanted to set up Ogre here so if there
	//were any errors then I could return 0 so that
	//the main thread would not continue.
	//However: Everything with Ogre should be done from the
	//same thread, so thats why the setup also happens in the seperate thread.

	//Create the ui thread
	if( pthread_create(&m_uithread, NULL, UIThreadStarter, this) ){
		std::cout << "[ERROR] pthread: pthread_create() failed!" << std::endl;
		return 0;
	}

	return 1;
}

int CUIMain::WaitForExit(void)
{
	pthread_join(m_uithread, NULL);
	return 1;
}


void* UIThreadStarter(void* class_pointer){
	return ((CUIMain*)class_pointer)->UIThread();
}

void* CUIMain::UIThread(void)
{
	//Setup Ogre
	int Success = 0;
	try{
		Success = SetupOgre();
	}catch( Ogre::Exception& e ) {
		std::cerr << "[ERROR] An exception has occurred while loading Ogre: \n" << e.what() << std::endl;
    }
	if( Success ){
		//Setup physics

		//Setup sound

		//Start the rendering loop
		mRoot->startRendering();
		//The rendering has ended: the user quit the window or an error occured
	}

	//Clean up
	CleanupOgre();

	return 0;
}

bool CUIMain::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool ContinueRendering = true;
	CMessage* msg = GetThreadMessage();
	if( msg ){ //If there are messages from other threads to process
		switch(msg->ID){
			//=================
			// General section
			//=================
			case Message_Quit:
				ContinueRendering = false;
				break;
			case Message_MsgBox:
				{
					CMessageMsgBox* msgbox = (CMessageMsgBox*)msg;
					mGUIHandler->MsgBox(msgbox->Text, msgbox->Title, msgbox->Buttons, msgbox->Callback, msgbox->WindowName);
				}
				break;
			case Message_DisplayWaitScreen:
				{
					CMessageDisplayWaitScreen* waitscreen = (CMessageDisplayWaitScreen*)msg;
					mGUIHandler->DisplayWaitScreen(waitscreen->Text);
				}
				break;
			case Message_CloseWaitScreen:
				mGUIHandler->CloseWaitScreen();
				break;
			//=================
			// Login section
			//=================
			case Message_NoWorld:
				{
					mGUIHandler->MsgBox("Unable to connect to the world server", "Error");
					mGUIHandler->DisplayLoginScreen("");
					mGUIHandler->CloseWaitScreen();
				}
				break;
			case Message_DisplayLoginScreen:
				{
					CMessageDisplayLoginScreen* loginmsg = (CMessageDisplayLoginScreen*)msg;
					mGUIHandler->DisplayLoginScreen(loginmsg->RememberedUsername);
					mGUIHandler->CloseWaitScreen();
				}
				break;
			case Message_LoginResponse:
				{
					CMessageLoginResponse* loginresponse = (CMessageLoginResponse*)msg;
					if( loginresponse->Code == ACK_ALREADY ){
						mGUIHandler->MsgBox("Your account is already logged in. Do you want to kick that account?", "Notice", MsgBoxBtnsYesNo, new MemberCallbackFunction<CUIMain>(&CUIMain::MsgBoxKickCallback, this));
					}else if( loginresponse->Code != ACK_SUCCESS ){
						std::string Message;
						switch(loginresponse->Code){
							case ACK_NOT_FOUND:
								Message = "The server was unable to find your username in the database.";
								break;
							case ACK_DOESNT_MATCH:
								Message = "Invalid password.";
								break;
							case ACK_REFUSED:
								Message = "The server explicitly refused your connection. You might be banned.";
								break;
							default:
								Message = "Error: invalid code in PCKT_W_AUTH_ACK.";
								break;
						}
						mGUIHandler->MsgBox(Message, "Error");
						mGUIHandler->DisplayLoginScreen("");
						mGUIHandler->CloseWaitScreen();
					}
				}
				break;
			case Message_DisplayCharSelect:
				{
					mGUIHandler->CloseLoginScreen();
					CMessageDisplayCharSelect* charmsg = (CMessageDisplayCharSelect*)msg;
					mGUIHandler->MsgBox("[Character select screen here]\nNot implemented yet, please quit.", "Not implemented yet");
				}
				break;
			default:
				break;
		}
		delete msg;
	}
	return ContinueRendering;
}

bool CUIMain::MsgBoxKickCallback(void* Param)
{
	bool DoKick = ((int)Param == CGUIHandler::MsgBoxBtnYes);
	CMainClient::getSingleton().SendThreadMessage(new CMessageKickAccount(DoKick));
	return true;
}