#include "UIMain.h"
#include "MainClient.h"
#include "GUIHandler.h"
#include "resProtocol.h"
#include <iostream>

CUIMain* CUIMain::mSingleton = 0;

CUIMain::CUIMain(void) : Ogre::FrameListener(), CThreadMessages(),
	Started(false), m_thread_id(0), m_thread_handle(0),
	mRoot(0), mCamera(0), mSceneMgr(0), mWindow(0), mRaySceneQuery(0),
	mInputHandler(0), mGUIHandler(0), mShowConsole(false)
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
	if (ACE_Thread::spawn(&UIThreadStarter, (void*)this, 0, &m_thread_id, &m_thread_handle) != 0) {
		std::cout << "[ERROR] ACE_Thread::spawn() failed!" << std::endl;
		return 0;
	}

	return 1;
}

int CUIMain::WaitForExit(void)
{
	ACE_THR_FUNC_RETURN return_value = 0;
	ACE_Thread::join(m_thread_handle, &return_value);
	//pthread_join(m_uithread, NULL);
	return 1;
}

ACE_THR_FUNC_RETURN CUIMain::UIThreadStarter(void* class_pointer)
{
	((CUIMain*)class_pointer)->UIThread();
	return 0;
}

int CUIMain::UIThread(void)
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
	MESSAGE msg = {0};
	GetThreadMessage(msg);
	switch(msg.ID){
		//=================
		// General section
		//=================
		case Message_Quit:
			ContinueRendering = false;
			break;
		case Message_MsgBox:
			{
				CMessageParamsMsgBox* msgbox = (CMessageParamsMsgBox*)msg.params;
				mGUIHandler->MsgBox(msgbox->Text, msgbox->Title, msgbox->Buttons, msgbox->Callback, msgbox->WindowName);
			}
			break;
		case Message_DisplayWaitScreen:
			{
				CMessageParamsDisplayWaitScreen* waitscreen = (CMessageParamsDisplayWaitScreen*)msg.params;
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
				CMessageParamsDisplayLoginScreen* loginmsg = (CMessageParamsDisplayLoginScreen*)msg.params;
				mGUIHandler->DisplayLoginScreen(loginmsg->RememberedUsername);
				mGUIHandler->CloseWaitScreen();
			}
			break;
		case Message_LoginResponse:
			{
				CMessageParamsLoginResponse* loginresponse = (CMessageParamsLoginResponse*)msg.params;
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
				CMessageParamsDisplayCharSelect* charmsg = (CMessageParamsDisplayCharSelect*)msg.params;
				mGUIHandler->MsgBox("[Character select screen here]\nNot implemented yet, please quit.", "Not implemented yet");
			}
			break;
		default:
			break;
	}
	if( msg.params ) delete msg.params;
	return ContinueRendering;
}

bool CUIMain::MsgBoxKickCallback(void* Param)
{
	bool DoKick = ((int)Param == CGUIHandler::MsgBoxBtnYes);
	CMainClient::getSingleton().SendThreadMessage(new CMessageParamsKickAccount(DoKick));
	return true;
}