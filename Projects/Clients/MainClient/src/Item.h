#pragma once
#include "entity.h"

class CItem :
	public CEntity
{
public:
	CItem(CWorldManager& World, Ogre::SceneNode *Node);
	virtual ~CItem(void);
	
	virtual const bool IsCItem(void) const{ return true; }
};
