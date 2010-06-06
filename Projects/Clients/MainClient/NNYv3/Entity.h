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

//The base class for all entities.
//The entity can be a visible object.
//Everything related to that can be gotten from the mNode member.
class CEntity
{
public:
	CEntity( EntityType Type ) : mEntityType(Type), Identifier(0), mNode(0), mMoveSpeed(0.0f), mDestinations()
	{
	}
	virtual ~CEntity(void)
	{
		////Ogre will clean up the nodes correctly so this should not be needed:
		//if( mNode ){
		//	//This method was not recommended.
		//	mNode->getCreator()->destroySceneNode(mNode);
		//}
	}

	inline void SetSceneNode(Ogre::SceneNode* Node){ mNode = Node; }
	inline Ogre::SceneNode* GetSceneNode(void){ return mNode; }

	inline const Ogre::Vector3 & GetPosition(void) const { return mNode->getPosition(); }
	Ogre::Vector3 GetMovement(void){
		if( mDestinations.size() == 0 ){
			return Ogre::Vector3(0,0,0);
		}else{
			Ogre::Vector3 Direction( (mDestinations.front() - GetPosition()).normalisedCopy() );
			return (Direction * mMoveSpeed);
		}
	}
	bool IsMoving(void){ return (mDestinations.size() != 0); }

	void SetMoveSpeed(Ogre::Real Speed){ mMoveSpeed = Speed; }
	Ogre::Real GetMoveSpeed(void){ return mMoveSpeed; }

	void AddDestination(Ogre::Vector3 Destination){ mDestinations.push(Destination); }
	void ReachedDestination(void){ mDestinations.pop(); }
	void ClearAllDestinations(void){ while( !mDestinations.empty() ){ mDestinations.pop(); } }
	void SetSingleDestination(Ogre::Vector3 Destination){ ClearAllDestinations(); AddDestination(Destination); }
	Ogre::Vector3 GetDestination(void){ if( mDestinations.empty() ) return Ogre::Vector3(0,0,0); else return mDestinations.front(); }

	//This is the identifier of an entity as used by the server-client communication
	unsigned int	Identifier;

protected:
	EntityType mEntityType;

	Ogre::SceneNode* mNode; //This will contain the position of the entity and hold the 3D mesh and so on
	//
	//Movement related
	//
	Ogre::Real mMoveSpeed; //To get move direction use GetMovement()
	//TODO: rotate-movement. Like in Flyff the items on the ground rotate.
	//What type do we need to hold rotation? Vector3 or Quaternion ?
	std::queue<Ogre::Vector3> mDestinations; //The positions the entity is heading to.
};