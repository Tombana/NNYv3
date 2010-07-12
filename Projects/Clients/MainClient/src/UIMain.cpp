#include "UIMain.h"
#include "MainClient.h"
#include "GUIHandler.h"
#include "resProtocol.h"
#include "ThreadMessagesConstants.h"
#include <iostream>

CUIMain* CUIMain::mSingleton = 0;

CUIMain::CUIMain(void) : Ogre::FrameListener(), CThreadMessages(), Thread(),
	Started(false),
	mRoot(0), mSceneMgr(0), mWindow(0), mCamera(), mQueryMouseMovement(0), mQueryMouseSelection(0),
	mInputHandler(0), mGUIHandler(0), mShowConsole(false),
	mWorld(), EntityNameCounter(0), mMoveDestinationIndicator(0), mEntityHoveringIndicator(0), mEntitySelectionIndicator(0)
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
	if( !start() ) return 0;
	return 1;
}

int CUIMain::WaitForExit(void)
{
	wait();
	return 1;
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
		mGUIHandler->DisplayWaitScreen("Loading... please wait");

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

	Ogre::Real ElapsedTime = evt.timeSinceLastFrame;

	//Test: show the 3D point that the cursor points to
	CEGUI::Point mousePos = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
	Ogre::Ray ray(mCamera.GetCamera()->getCameraToViewportRay(mousePos.d_x, mousePos.d_y));
	bool MouseOnEntity = false;
	
	mQueryMouseSelection->setRay(ray);
	Ogre::RaySceneQueryResult& qryResult = mQueryMouseSelection->execute();
	for( Ogre::RaySceneQueryResult::iterator it = qryResult.begin(); it != qryResult.end(); ++it ){
		if( !it->movable ) continue;
		CEntity* Entity = static_cast<CEntity*>( it->movable->getUserObject() );
		if( !Entity ) continue;
		MouseOnEntity = true;
		mEntityHoveringIndicator->setPosition(Entity->GetPosition());
		break;
	}
	if( !MouseOnEntity ) {
		mQueryMouseMovement->setRay(ray);
		Ogre::RaySceneQueryResult& qryResult = mQueryMouseMovement->execute();
		for( Ogre::RaySceneQueryResult::iterator it = qryResult.begin(); it != qryResult.end(); ++it ){
			//if( it->movable && it->movable == mMoveDestinationIndicator->getAttachedObjectIterator().getNext() ) continue;
			Ogre::Vector3 CollisionPoint = ray.getPoint(it->distance);
			mMoveDestinationIndicator->setPosition(CollisionPoint);
			break;
		}
	}
	mEntityHoveringIndicator->setVisible( MouseOnEntity );
	mMoveDestinationIndicator->setVisible( !MouseOnEntity );


	//=================================
	// Update entities (movement, animations, effects)
	// Every entity is updated individually
	// 1. If it is moving, its position is updated
	// 2. Its virtual Update() method is called with elapsed time
	// 3. Its animation is updated (advanced to next frame)
	//=================================
	for(CWorldManager::EntityList::iterator ent = mWorld.mEntities.begin(); ent != mWorld.mEntities.end(); ++ent ){
		CEntity *Entity = ent->second;
		if( Entity == 0 ) continue; //Shouldn't happen but for safety
		//
		// 1. Update position if moving
		//
		Entity->UpdateMovement(ElapsedTime);
		//
		// 2. Call the Update() method
		//
		Entity->Update(ElapsedTime);
		//
		// 3. Update animations
		//
		Entity->UpdateAnimations(ElapsedTime);
	}

	//=================================
	// Update camera position, smooth scrolling and zooming
	//=================================
	mCamera.Update(ElapsedTime, mWorld.LocalPlayer->GetPosition());

	//=================================
	// Messages sent to the gui thread
	//=================================
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
				mGUIHandler->DisplayWorldSelect(true);
				mGUIHandler->CloseWaitScreen();
			}
			break;
		case Message_DisplayWorldSelect:
			{
				CMessageParamsDisplayWorldSelect* servermsg = (CMessageParamsDisplayWorldSelect*)msg.params;
				mGUIHandler->DisplayWorldSelect(servermsg->Servers);
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

CPlayer* CUIMain::CreateNewPlayer(unsigned int Identifier, const CharacterInfo& characterinfo)
{
	Ogre::SceneNode *MainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	CPlayer* Player = new CPlayer(mWorld, MainNode);
	AttachMeshes(Player, characterinfo);

	Player->SetState(State_Idle);
	//Set movespeed and so on based on characterinfo
	Player->SetMoveSpeed(50);
	return Player;
}

void CUIMain::AttachMeshes(CEntity* Entity, const CharacterInfo& characterinfo)
{
	Ogre::String EntityNameSuffix = Ogre::StringConverter::toString(EntityNameCounter);
	++EntityNameCounter;

	Ogre::SceneNode* MainNode = Entity->GetSceneNode();
	MainNode->setFixedYawAxis(true);

	//For every part of the entity (body,head,weapons,clothes) create a seperate node
	//TODO: Do this based on characterinfo and stuff from datafiles (like xml files)
	Ogre::SceneNode *bodynode = MainNode->createChildSceneNode();
	//Ogre::SceneNode *headnode = MainNode->createChildSceneNode();
	Ogre::Entity *body = mSceneMgr->createEntity("Entity_Body_" + EntityNameSuffix, "goku.mesh");
	//Ogre::Entity *head = mSceneMgr->createEntity("Entity_Head_" + EntityNameSuffix, "ogrehead.mesh");
	bodynode->attachObject(body);
	//headnode->attachObject(head);
	//headnode->yaw(Ogre::Degree(180));
	bodynode->yaw(Ogre::Degree(180));
	bodynode->setPosition(Ogre::Vector3(0,5,0));
	//headnode->scale(0.3,0.3,0.3);
	//headnode->setPosition(0, 80, -5);

	body->setUserObject(Entity);
	//head->setUserObject(Entity);

	Ogre::MeshPtr bodymesh = body->getMesh();
	Ogre::AxisAlignedBox boundingbox = bodymesh->getBounds();
	boundingbox.scale(Ogre::Vector3(0.7,1,0.9));
	bodymesh->_setBounds( boundingbox );

	//headnode->showBoundingBox(true);
	bodynode->showBoundingBox(true);

	//Make sure to do this to every (sub)entity that belongs to a player
	body->setQueryFlags(QUERY_MASK_MOUSE_SELECTING);
	//head->setQueryFlags(QUERY_MASK_MOUSE_SELECTING);

	//Animation names have to be taken from some data file (like xml).
	Entity->Animations[State_Idle].push_back( body->getAnimationState("Idle") );
	//Entity->Animations[State_Idle].push_back( head->getAnimationState("Idle") ); //Add the animations for every mesh
	Entity->Animations[State_Moving].push_back( body->getAnimationState("Move") );
	Entity->Animations[State_MovingFast].push_back( body->getAnimationState("Move_Fast") );
	Entity->Animations[State_Fighting].push_back( body->getAnimationState("Fight") );

	return;
}