#include "LocalPlayer.h"
#include "Ogre\EntityMaterialInstance.h"

CLocalPlayer::CLocalPlayer(Ogre::SceneNode *Node) : CPlayer(Node),
	DestinationMarker(0), MarkerEntity(0), CurrentAlpha(0), TotalMoveTime(0), MoveTimePassed(0), FadeInTime(0.5), FadeOutTime(1.0), FadeTimeRatio(FadeInTime/(FadeInTime+FadeOutTime))
{
}

CLocalPlayer::~CLocalPlayer(void)
{
	if( MarkerEntity ) delete MarkerEntity;
}

void CLocalPlayer::SetDestinationMarker(Ogre::SceneNode* Node, Ogre::Entity* BlendingEntity)
{
	//Clean up the old node
	if( MarkerEntity ) delete MarkerEntity;
	DestinationMarker = Node;
	MarkerEntity = new EntityMaterialInstance(BlendingEntity);
}

void CLocalPlayer::AddDestination(Ogre::Vector3 Destination)
{
	CPlayer::AddDestination(Destination);
	if( DestinationMarker ){
		TotalMoveTime = GetMoveTimeLeft();
		MoveTimePassed = 0;
		DestinationMarker->setPosition(mDestinations.back()); //The last destination is the marker position
		DestinationMarker->setVisible(true);
	}
}

void CLocalPlayer::ReachedDestination(void)
{
	CPlayer::ReachedDestination();
	if( DestinationMarker && mDestinations.empty() ) DestinationMarker->setVisible(false);
}

void CLocalPlayer::ClearAllDestinations(void)
{
	CPlayer::ClearAllDestinations();
	if( DestinationMarker ) DestinationMarker->setVisible(false);
}

void CLocalPlayer::Update(Ogre::Real ElapsedTime)
{
	CPlayer::Update(ElapsedTime);
	if( IsMoving() ){ //Destination marker alpha update
		//
		// When walking by clicking: a destination marker
		// When you click:
		// The marker will fade in the first half second
		// The marker will fade out the last second
		// The marker will be fully visible inbetween
		// When you click somewhere that is less than 1.5 second away:
		// It will divide the time into two parts with the same ratio as the fade-in and fade-out time.
		// The first of those parts will fade in, the second part will fade out.
		//
		MoveTimePassed += ElapsedTime;
		Ogre::Real TimeLeft = TotalMoveTime - MoveTimePassed;
		if( TotalMoveTime >= (FadeInTime + FadeOutTime) ){ //Total time is more than 1 second
			//So first half second fade in, last half second fade out
			if( MoveTimePassed < FadeInTime ) CurrentAlpha = MoveTimePassed/FadeInTime;
			else if( TimeLeft < FadeOutTime ) CurrentAlpha = TimeLeft/FadeOutTime;
			else CurrentAlpha = 1;
		}else{
			Ogre::Real FadeIn = TotalMoveTime * FadeTimeRatio;
			Ogre::Real FadeOut = TotalMoveTime - FadeIn;
			if( MoveTimePassed < FadeIn ) CurrentAlpha = MoveTimePassed/FadeInTime;
			else if( TimeLeft < FadeOut ) CurrentAlpha = TimeLeft/FadeOutTime;
		}
		
		MarkerEntity->setTransparency(1 - CurrentAlpha);
	}
}