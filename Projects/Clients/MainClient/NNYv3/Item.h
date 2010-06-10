#pragma once
#include "entity.h"

class CItem :
	public CEntity
{
public:
	CItem(Ogre::SceneNode *Node);
	virtual ~CItem(void);
};
