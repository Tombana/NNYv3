#pragma once
#include "entity.h"

class CNpc :
	public CEntity
{
public:
	CNpc(CWorldManager& World, Ogre::SceneNode *Node);
	virtual ~CNpc(void);

	virtual const bool IsCNpc(void) const{ return true; }
};
