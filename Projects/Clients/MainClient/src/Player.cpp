#include "Player.h"

CPlayer::CPlayer(CWorldManager& World, Ogre::SceneNode *Node) : CCombatEntity( World, EntityType_Player, Node )
{
}

CPlayer::~CPlayer(void)
{
}
