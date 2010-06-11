#include "Camera.h"

CCamera::CCamera(void) : mSceneManager(0), mCamera(0), mCamNode(0), mCamDist(200), MinCamDist(100), MaxCamDist(800), mCamZoomSpeed(0), mCamYawSpeed(0), mCamPitchSpeed(0)
{
}

CCamera::~CCamera(void)
{
}

//Create the camera object
void CCamera::Initialize(Ogre::SceneManager* SceneManager)
{
	mSceneManager = SceneManager;
	if( mCamera == 0 ){
		mCamera = mSceneManager->createCamera("PlayerCam");
		mCamera->setNearClipDistance(5);
		
		//The CamNode will be the point that the camera rotates around, so a point at the player
		mCamNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");
		mCamNode->attachObject(mCamera);
		mCamera->setPosition(Ogre::Vector3(0,0,mCamDist)); //Set the camera 100 units in front the node
		mCamera->lookAt(Ogre::Vector3(0,0,0)); //Look at the node
	}
}

//Delete the camera object
void CCamera::Shutdown(void)
{
	if( mSceneManager ){
		if( mCamera ) mSceneManager->destroyCamera(mCamera);
		if( mCamNode ) mSceneManager->destroySceneNode(mCamNode);
	}
	mCamNode = 0;
	mCamera = 0;
	mSceneManager = 0;
}

Ogre::Camera* CCamera::GetCamera(void)
{
	return mCamera;
}

void CCamera::Update(Ogre::Real ElapsedTime, Ogre::Vector3 PlayerPosition)
{
	//1. Camera position
	Ogre::Vector3 CamPos = mCamNode->getPosition();
	Ogre::Vector3 ToMove = PlayerPosition - CamPos;
	Ogre::Real Dist = ToMove.length();
	if( Dist > 0 ){ //If the camera node is not at the player
		if( Dist > 100 ){ //Camera is more than 100 units away, so just teleport it to the player
			mCamNode->setPosition(PlayerPosition);
		}else{ //Normal distance, so move camera with normal speed
			//Speed depends on distance
			CamPos += ToMove * (Dist * 0.05f * ElapsedTime);
			mCamNode->setPosition(CamPos);
		}
	}

	//2. Camera rotation
	if( mCamYawSpeed ){
		mCamNode->yaw(Ogre::Degree(mCamYawSpeed * ElapsedTime), Ogre::Node::TS_WORLD);
		//Decrease the speed (which causes a natural 'slow down')
		DeAccelerate(mCamYawSpeed, 200 * ElapsedTime); //Deacceleration of 200 degrees per second per second
	}
	if( mCamPitchSpeed ){
		mCamNode->pitch(Ogre::Degree(mCamPitchSpeed * ElapsedTime), Ogre::Node::TS_LOCAL);
		//Decrease the speed (which causes a natural 'slow down')
		DeAccelerate(mCamPitchSpeed, 200 * ElapsedTime); //Deacceleration of 200 degrees per second per second
	}

	//3. Camera zoom
	if( mCamZoomSpeed ){
		//Adjust the zoom with the speed
		mCamDist += mCamZoomSpeed * ElapsedTime;
		//Make sure it does not zoom in or out too far
		if( mCamDist < MinCamDist ){ mCamDist = MinCamDist; mCamZoomSpeed = 0; }
		else if( mCamDist > MaxCamDist ){ mCamDist = MaxCamDist; mCamZoomSpeed = 0; }
		//Decrease the speed (which causes a natural 'slow down')
		DeAccelerate(mCamZoomSpeed, 500 * ElapsedTime); //Deacceleration of 500 units per second per second
		//Apply the new zoom distance
		mCamera->setPosition(Ogre::Vector3(0,0,mCamDist));
	}
}

void CCamera::DeAccelerate(Ogre::Real& Speed, Ogre::Real Acceleration)
{
	if( Speed > 0 ){
		if( Speed < Acceleration ) Speed = 0;
		else Speed -= Acceleration;
	}else{
		if( Speed > Acceleration ) Speed = 0;
		else Speed += Acceleration;
	}
}