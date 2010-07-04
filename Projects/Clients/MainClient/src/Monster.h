#pragma once
#include "combatentity.h"

class CMonster :
	public CCombatEntity
{
public:
	CMonster(CWorldManager& World, Ogre::SceneNode *Node);
	virtual ~CMonster(void);
};
