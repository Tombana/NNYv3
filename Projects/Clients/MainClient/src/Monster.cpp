#include "Monster.h"

CMonster::CMonster(CWorldManager& World, Ogre::SceneNode *Node) : CCombatEntity(World, EntityType_Monster, Node)
{
}

CMonster::~CMonster(void)
{
}
