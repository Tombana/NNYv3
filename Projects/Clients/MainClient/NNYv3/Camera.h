#pragma once

#include <Ogre.h>

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	//Create the camera object
	void Initialize(Ogre::SceneManager* SceneManager);
	//Delete the camera object
	void Shutdown(void);

	//Get the ogre camera object
	Ogre::Camera* GetCamera(void);

	//Update position, rotation and zoom
	void Update(Ogre::Real ElapsedTime, Ogre::Vector3 PlayerPosition);

	//These are written to by CInputHandler
	Ogre::Real			mCamZoomSpeed; //Zoomspeed. positive means zooming out
	Ogre::Real			mCamYawSpeed; //Yaw speed in degrees per second
	Ogre::Real			mCamPitchSpeed; //Pitch speed in degrees per second

private:
	Ogre::SceneManager	*mSceneManager;
	Ogre::Camera		*mCamera;
	Ogre::SceneNode		*mCamNode; //The CamNode will be the point that the camera rotates around, so a point at the player
	Ogre::Real			mCamDist; //Zoom. Higher means further away
	Ogre::Real			MinCamDist;
	Ogre::Real			MaxCamDist;

	void DeAccelerate(Ogre::Real& Speed, Ogre::Real Acceleration);
};
