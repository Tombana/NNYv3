#pragma once
#include "entity.h"

class CCombatEntity :
	public CEntity
{
public:
	CCombatEntity( CWorldManager& World, EntityType Type, Ogre::SceneNode *Node );
	virtual ~CCombatEntity(void);

	virtual const bool IsCCombatEntity(void) const{ return true; }

	//TODO: Write this? Or let the server handle it.
	//Second param: struct AttackMethod (struct that holds attack range)
	bool IsWithinAttackRange(CCombatEntity* Target, void*){
		if( GetDistance(Target) < 100 ) return true;
		return false;
	}

	typedef std::vector<CCombatEntity*> FightTargetList;
	FightTargetList FightTargets;
};
