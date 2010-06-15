#pragma once
#include "player.h"

//So we don't have to include the whole file
class EntityMaterialInstance;

class CLocalPlayer :
	public CPlayer
{
public:
	CLocalPlayer(Ogre::SceneNode *Node);
	virtual ~CLocalPlayer(void);

	void SetDestinationMarker(Ogre::SceneNode* Node, Ogre::Entity* BlendingEntity);

	//To enable/disable the destination marker
	virtual void AddDestination(Ogre::Vector3 Destination);
	virtual void ReachedDestination(void);
	virtual void ClearAllDestinations(void);

	//This will update the destination marker alpha value
	virtual void Update(Ogre::Real ElapsedTime);

protected:
	//================================================
	// When walking by clicking: a destination marker
	// When you click:
	// The marker will fade in the first half second
	// The marker will fade out the last second
	// The marker will be fully visible inbetween
	// When you click somewhere that is less than 1.5 second away:
	// It will divide the time into two parts with the same ratio as the fade-in and fade-out time.
	// The first of those parts will fade in, the second part will fade out.
	//
	Ogre::SceneNode *DestinationMarker;
	EntityMaterialInstance *MarkerEntity; //For alpha blending
	Ogre::Real CurrentAlpha;
	Ogre::Real TotalMoveTime; //The time in seconds from when clicked untill destination. Set when clicking
	Ogre::Real MoveTimePassed; //Time passed since click
	const Ogre::Real FadeInTime; //0,5 seconds
	const Ogre::Real FadeOutTime; //1,0 seconds
	const Ogre::Real FadeTimeRatio; //So it only has to be calculated once. = FadeInTime/(FadeInTime + FadeOutTime)
	//
	void UpdateMarkerAlpha(void);
	//================================================

};
