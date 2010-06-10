#pragma once
#include "entity.h"

class CNpc :
	public CEntity
{
public:
	CNpc(Ogre::SceneNode *Node);
	virtual ~CNpc(void);
};
