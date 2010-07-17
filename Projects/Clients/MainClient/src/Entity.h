#pragma once

#include <Ogre.h>
#include <queue>

//Can not include the file here because that would result in a loop of includes
class CWorldManager;

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

typedef enum EntityState{ State_Disabled = 0, State_Idle, State_Moving, State_MovingFast, State_Fighting, State_MAXENTITYSTATE } ;

//The base class for all entities.
//The entity can be a visible object.
//Everything related to that can be gotten from the mNode member.
class CEntity : public Ogre::UserDefinedObject //UserDefinedObject will let CEntity link with Ogre::Entity
{
public:
	CEntity( CWorldManager& World, EntityType Type, Ogre::SceneNode *Node );
	virtual ~CEntity(void);

	virtual long getTypeID(void) const{ return mEntityType; } //From Ogre::UserDefinedObject
	virtual const bool IsCItem(void) const{ return false; }
	virtual const bool IsCNpc(void) const{ return false; }
	virtual const bool IsCCombatEntity(void) const{ return false; }
	virtual const bool IsCMonster(void) const{ return false; }
	virtual const bool IsCPlayer(void) const{ return false; }
	virtual const bool IsCLocalPlayer(void) const{ return false; }

	//Sets the state and returns the old state.
	//Will set the appropriate animations
	EntityState SetState(EntityState NewState);
	EntityState GetState(void){ return mState; }

	inline Ogre::SceneNode* GetSceneNode(void){ return mNode; }

	inline unsigned int GetIdentifier(void){ return mIdentifier; }
	//This will update the Identifier in CWorldManager as well
	void SetIdentifier(unsigned int Identifier);

	//Move speed
	void SetMoveSpeed(Ogre::Real Speed){ mMoveSpeed = Speed; }
	Ogre::Real GetMoveSpeed(void){ return mMoveSpeed; }

	//Position
	inline const Ogre::Vector3 & GetPosition(void) const { return mNode->getPosition(); }
	Ogre::Real GetDistance( CEntity* Entity ) const { return (GetPosition() - Entity->GetPosition()).length(); }

	//
	//Movement
	//
	//This will return the first destination point.
	//If following a target it will return the position at which this entity
	// would stop if the target was not moving. See mRaidus for details.
	Ogre::Vector3 GetDestination(void);
	//Get the normalized move direction times the move speed
	Ogre::Vector3 GetMovement(void); //returns Direction * Speed
	bool IsMoving(void){ return (mState == State_Moving); }
	bool IsFollowing(void){ return (IsMoving() && mFollowing && mDestinations.empty()); }
	CEntity* GetFollowing(void){ return mFollowing; }
	//For fixed-point destination only:
	Ogre::Real GetDistanceLeft(void);
	Ogre::Real GetMoveTimeLeft(void);

	//Change movement
	void StopMoving(void){ ClearAllDestinations(); FollowEntity(0); SetState(State_Idle); }
	//
	//Change movement - fixed point destination
	//
	virtual void AddDestination(Ogre::Vector3 Destination);
	virtual void ReachedDestination(void);
	virtual void ClearAllDestinations(void){ while( !mDestinations.empty() ){ mDestinations.pop(); } }
	void SetSingleDestination(Ogre::Vector3 Destination){ ClearAllDestinations(); AddDestination(Destination); }

	//
	//Change movement - following an entity
	//
	//Pass zero to stop following. Passing an entity will clear the fixed-point destination
	bool FollowEntity(CEntity* Entity);

	//For every EntityState there is a list of AnimationState's that belongs to it.
	typedef std::vector<Ogre::AnimationState*> AnimList;
	AnimList Animations[State_MAXENTITYSTATE];

	//These three functions are only to be called by CUIMain at each frame
	virtual void Update(Ogre::Real ElapsedTime){};
	void UpdateMovement(Ogre::Real ElapsedTime);
	void UpdateAnimations(Ogre::Real ElapsedTime);

	//This is used for:
	//When A follows B, it stops when Distance(A,B) == RadiusA + RadiusB
	//A starts following B again when Distance(A,B) == RadiusPadA + RadiusPadB
	//The second one should be bigger to prevent the following problem:
	//if A is faster than B:
	// 1. A reaches B
	// 2. A goes into 'Idle'
	// 3. B moves another millimeter
	// 4. A goes into 'Move' again
	// 5. A reaches B because A is faster
	// 6. Goto step 2.
	// This will repeat very quickly resulting in fast changes between the animations which looks laggy
	Ogre::Real mRadius;
	Ogre::Real mRadiusPad; 

protected:
	CWorldManager& mWorld;

	const EntityType mEntityType;

	EntityState mState;

	//This is the identifier of an entity as used by the server-client communication
	unsigned int mIdentifier;

	Ogre::SceneNode *mNode; //This will contain the position of the entity and hold a hiarchy with the 3D mesh and so on
	//
	//Movement related
	//
	Ogre::Real mMoveSpeed; //Speed in units per second
	//TODO: rotate-movement. Like in Flyff the items on the ground rotate.
	//What type do we need to hold rotation? Vector3 or Quaternion ?

	//Move destinations: either the entity is following another entity or it is
	//heading to a fixed destination.
	//If both of these are set (should not be possible) then the fixed destination goes first!
	std::queue<Ogre::Vector3> mDestinations; //The positions the entity is heading to.
	CEntity* mFollowing; //Zero when not following. Might change this to Ogre::SceneNode to be able to follow other things.

	Ogre::Vector3 GetFollowDestination(void); //Also implements mRadius
};