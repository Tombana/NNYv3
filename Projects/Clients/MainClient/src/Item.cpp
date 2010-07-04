#include "Item.h"

CItem::CItem(CWorldManager& World, Ogre::SceneNode *Node) : CEntity(World, EntityType_Item, Node)
{
}

CItem::~CItem(void)
{
}
