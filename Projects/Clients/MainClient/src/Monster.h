#pragma once
#include "combatentity.h"

class CMonster :
	public CCombatEntity
{
public:
	CMonster(Ogre::SceneNode *Node);
	virtual ~CMonster(void);
};
