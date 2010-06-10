#include "Entity.h"

CEntity::CEntity( EntityType Type, Ogre::SceneNode *Node ) :
	mEntityType(Type), mNode(Node), Identifier(0),
	AnimIdle(0), AnimWalk(0), AnimFight(0),
	mMoveSpeed(0.0f), mDestinations()
{
	SetState(State_Disabled);
}

EntityState CEntity::SetState(EntityState NewState)
{
	EntityState OldState = mState;
	mState = NewState;
	switch( mState ){
		case State_Idle:
			if( AnimIdle ) AnimIdle->setEnabled(true);
			if( AnimWalk ) AnimWalk->setEnabled(false);
			break;
		case State_Walking:
			if( AnimIdle ) AnimIdle->setEnabled(false);
			if( AnimWalk ) AnimWalk->setEnabled(true);
			break;
		default:
			break;
	}
	return OldState;
}

void CEntity::UpdateMovement(Ogre::Real ElapsedTime)
{
	//Also update walk/idle animation?
	if( IsMoving() ){
		Ogre::Real LenghtLeftToGo = (GetDestination() - GetPosition()).length();
		Ogre::Vector3 Movement = GetMovement();
		Ogre::Vector3 CorrectedMovement = ( Movement * ElapsedTime );

		//Set the right angle for the entity
		mNode->lookAt( GetDestination(), Ogre::Node::TS_WORLD );

		if( CorrectedMovement.length() < LenghtLeftToGo ){ //Not at destination yet, just move
			mNode->translate( CorrectedMovement );
		}else{ //Arrived at destination
			mNode->setPosition( GetDestination() );
			ReachedDestination();
			//TODO: If there is a next destination then go there with the frametime left of this movement.
			//(Loop till all frametime is used for movement)
			//Example: if there are 3 destinations left, and the first 2 will be reached
			//in 2 seconds.
			//If the user has a slow computer that updates the frame every 2,5 seconds,
			//then it should first use x seconds to reach destination one, then check
			//for how many seconds left, and use those to go to the next node and so on.
		}
	}
}

//This is only to be called by CUIMain
void CEntity::UpdateAnimations(Ogre::Real ElapsedTime)
{
	//TODO: This obviously has to be some vector or array with all AnimationStates
	if( AnimIdle && AnimIdle->getEnabled() && !AnimIdle->hasEnded() ) AnimIdle->addTime(ElapsedTime);
	if( AnimWalk && AnimWalk->getEnabled() && !AnimWalk->hasEnded() ) AnimWalk->addTime(ElapsedTime);
	if( AnimFight && AnimFight->getEnabled() && !AnimFight->hasEnded() ) AnimFight->addTime(ElapsedTime);
}

Ogre::Vector3 CEntity::GetMovement(void)
{
	if( mDestinations.size() == 0 ){
		return Ogre::Vector3(0,0,0);
	}else{
		Ogre::Vector3 Direction( (mDestinations.front() - GetPosition()).normalisedCopy() );
		return (Direction * mMoveSpeed);
	}
}

void CEntity::AddDestination(Ogre::Vector3 Destination)
{
	mDestinations.push(Destination);
	SetState(State_Walking);
}

void CEntity::ReachedDestination(void)
{
	mDestinations.pop();
	if( mDestinations.empty() ) SetState(State_Idle);
}