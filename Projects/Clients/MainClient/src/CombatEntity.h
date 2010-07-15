#pragma once
#include "entity.h"

class CCombatEntity :
	public CEntity
{
public:
	CCombatEntity( CWorldManager& World, EntityType Type, Ogre::SceneNode *Node );
	virtual ~CCombatEntity(void);

	virtual const bool IsCCombatEntity(void) const{ return true; }

	typedef std::vector<CCombatEntity*> FightTargetList;
	FightTargetList FightTargets;
};
