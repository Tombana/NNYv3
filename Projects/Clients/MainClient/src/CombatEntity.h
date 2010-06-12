#pragma once
#include "entity.h"

class CCombatEntity :
	public CEntity
{
public:
	CCombatEntity( EntityType Type, Ogre::SceneNode *Node );
	virtual ~CCombatEntity(void);
};
