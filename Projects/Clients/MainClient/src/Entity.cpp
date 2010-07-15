#include "Entity.h"
#include "WorldManager.h"

CEntity::CEntity( CWorldManager& World, EntityType Type, Ogre::SceneNode *Node ) :
	mWorld(World), mEntityType(Type), mNode(Node), mState(State_Disabled), mIdentifier(0), Animations(),
	mMoveSpeed(0.0f), mDestinations()
{
	SetState(State_Disabled);
	mWorld.RegisterEntity(mIdentifier, this);
}

CEntity::~CEntity(void)
{
	////Ogre will clean up the nodes correctly so this should not be needed:
	//if( mNode ){
	//	//This method was not recommended.
	//	mNode->getCreator()->destroySceneNode(mNode);
	//}
	mWorld.UnregisterEntity(this);
}

void CEntity::SetIdentifier(unsigned int Identifier)
{
	mIdentifier = Identifier;
	mWorld.ChangeIdentifier(this, Identifier);
}

EntityState CEntity::SetState(EntityState NewState)
{
	EntityState OldState = mState;
	mState = NewState;

	//If it actually changed
	if( mState != OldState ){
		//Disable all old animations
		if( !Animations[OldState].empty() ){
			for( AnimList::iterator it = Animations[OldState].begin(); it != Animations[OldState].end(); ++it ){
				(*it)->setEnabled(false);
				(*it)->setTimePosition(0);
			}
		}
		//Enable all new animations
		if( !Animations[mState].empty() ){
			for( AnimList::iterator it = Animations[mState].begin(); it != Animations[mState].end(); ++it )
				(*it)->setEnabled(true);
		}
	}

	if( mState != State_Moving ) ClearAllDestinations(); //Or remember the destinations?

	//Do other stuff that is required
	//See TODO CEntity::UpdateAnimations: maybe that should be in this switch case
	switch( mState ){
		case State_Idle:
			break;
		case State_Moving:
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
	//TODO: if mState == State_Moving then the movespeed should be a multiplier on ElapsedTime
	//TODO: if mState == State_Fighting then the attackspeed should be a multiplier on ElapsedTime
	if( !Animations[mState].empty() ){
		for( AnimList::iterator it = Animations[mState].begin(); it != Animations[mState].end(); ++it ){
			if( !(*it)->hasEnded() ) (*it)->addTime(ElapsedTime);
		}
	}
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

Ogre::Real CEntity::GetDistanceLeft(void)
{
	//TODO: It should actually iterate through the destinations and get the total distance.
	if( mDestinations.size() == 0 ) return 0;
	return (mDestinations.front() - GetPosition()).length();
}

Ogre::Real CEntity::GetMoveTimeLeft(void)
{
	if( !IsMoving() ) return 0;
	return GetDistanceLeft() / mMoveSpeed;
}

void CEntity::AddDestination(Ogre::Vector3 Destination)
{
	mDestinations.push(Destination);
	SetState(State_Moving);
}

void CEntity::ReachedDestination(void)
{
	mDestinations.pop();
	if( mDestinations.empty() ) SetState(State_Idle);
}