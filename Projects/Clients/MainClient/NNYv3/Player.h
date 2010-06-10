#pragma once
#include "combatentity.h"

class CPlayer :
	public CCombatEntity
{
public:
	CPlayer(Ogre::SceneNode *Node);
	virtual ~CPlayer(void);
};
