#pragma once

#include <Ogre.h>
#include <queue>

//The entity hiarchy:
//CEntity
// |- CItem
// |- CNpc
// |- CCombatEntity
//     |- CMonster
//     |- CPlayer
//        |- CLocalPlayer

//To determine what class an entity is (CPlayer, CMonster and so on)
typedef enum EntityType{	EntityType_Unkown = 0,
							EntityType_Item,
							EntityType_Player,
							EntityType_Monster,
							EntityType_NPC };

typedef enum EntityState{ State_Disabled = 0, State_Idle, State_Walking, State_Fighting } ;

//The base class for all entities.
//The entity can be a visible object.
//Everything related to that can be gotten from the mNode member.
class CEntity
{
public:
	CEntity( EntityType Type, Ogre::SceneNode *Node );
	virtual ~CEntity(void)
	{
		////Ogre will clean up the nodes correctly so this should not be needed:
		//if( mNode ){
		//	//This method was not recommended.
		//	mNode->getCreator()->destroySceneNode(mNode);
		//}
	}

	//Sets the state and returns the old state.
	//Will set the appropriate animations
	EntityState SetState(EntityState NewState);
	EntityState GetState(void){ return mState; }

	//TODO: Implement this function
	virtual void Update(Ogre::Real ElapsedTime){};

	//This is only to be called by CUIMain
	void UpdateMovement(Ogre::Real ElapsedTime);
	//This is only to be called by CUIMain
	void UpdateAnimations(Ogre::Real ElapsedTime);

	inline Ogre::SceneNode* GetSceneNode(void){ return mNode; }

	void SetMoveSpeed(Ogre::Real Speed){ mMoveSpeed = Speed; }
	Ogre::Real GetMoveSpeed(void){ return mMoveSpeed; }

	inline const Ogre::Vector3 & GetPosition(void) const { return mNode->getPosition(); }

	Ogre::Vector3 GetMovement(void);
	bool IsMoving(void){ return (mState == State_Walking); }

	void AddDestination(Ogre::Vector3 Destination);
	void ReachedDestination(void);
	void ClearAllDestinations(void){ while( !mDestinations.empty() ){ mDestinations.pop(); } }
	void SetSingleDestination(Ogre::Vector3 Destination){ ClearAllDestinations(); AddDestination(Destination); }
	Ogre::Vector3 GetDestination(void){ if( mDestinations.empty() ) return Ogre::Vector3(0,0,0); else return mDestinations.front(); }

	//This is the identifier of an entity as used by the server-client communication
	unsigned int	Identifier;

	//These can be filled with animations
	Ogre::AnimationState *AnimIdle;
	Ogre::AnimationState *AnimWalk;
	Ogre::AnimationState *AnimFight;

protected:
	EntityType mEntityType;

	EntityState mState;

	Ogre::SceneNode *mNode; //This will contain the position of the entity and hold a hiarchy with the 3D mesh and so on
	//
	//Movement related
	//
	Ogre::Real mMoveSpeed; //Speed in units per second
	//TODO: rotate-movement. Like in Flyff the items on the ground rotate.
	//What type do we need to hold rotation? Vector3 or Quaternion ?
	std::queue<Ogre::Vector3> mDestinations; //The positions the entity is heading to.
};