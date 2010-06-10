#pragma once
#include "player.h"

class CLocalPlayer :
	public CPlayer
{
public:
	CLocalPlayer(Ogre::SceneNode *Node);
	virtual ~CLocalPlayer(void);
};
