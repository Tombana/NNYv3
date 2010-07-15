#pragma once
#include "combatentity.h"

class CPlayer :
	public CCombatEntity
{
public:
	CPlayer(CWorldManager& World, Ogre::SceneNode *Node);
	virtual ~CPlayer(void);

	virtual const bool IsCPlayer(void) const{ return true; }
};
