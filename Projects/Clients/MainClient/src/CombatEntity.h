#pragma once
#include "entity.h"

class CCombatEntity :
	public CEntity
{
public:
	CCombatEntity( CWorldManager& World, EntityType Type, Ogre::SceneNode *Node );
	virtual ~CCombatEntity(void);
};
