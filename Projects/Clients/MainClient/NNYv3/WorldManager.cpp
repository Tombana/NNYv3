#include "WorldManager.h"

CWorldManager::CWorldManager(void) : mEntities(), LocalPlayer(0)
{
}

CWorldManager::~CWorldManager(void)
{
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		delete *ent;
	}
	mEntities.clear();
}

CItem* CWorldManager::CreateItem(unsigned int Identifier, Ogre::SceneNode *Node)
{
	return (CItem*)CreateEntity(EntityType_Item, Identifier, Node);
}

CPlayer* CWorldManager::CreatePlayer(unsigned int Identifier, Ogre::SceneNode *Node)
{
	return (CPlayer*)CreateEntity(EntityType_Player, Identifier, Node);
}

CMonster* CWorldManager::CreateMonster(unsigned int Identifier, Ogre::SceneNode *Node)
{
	return (CMonster*)CreateEntity(EntityType_Monster, Identifier, Node);
}

CNpc* CWorldManager::CreateNPC(unsigned int Identifier, Ogre::SceneNode *Node)
{
	return (CNpc*)CreateEntity(EntityType_NPC, Identifier, Node);
}

CEntity* CWorldManager::CreateEntity(EntityType Type, unsigned int Identifier, Ogre::SceneNode *Node)
{
	//Check if it already exists
	if( GetEntityFromIdentifier(Identifier) ) return 0;
	CEntity* ret = 0;
	switch(Type){
		case EntityType_Item:
			ret = new CItem(Node);
			break;
		case EntityType_Player:
			ret = new CPlayer(Node);
			break;
		case EntityType_Monster:
			ret = new CMonster(Node);
			break;
		case EntityType_NPC:
			ret = new CNpc(Node);
			break;
		default:
			break;
	}
	if( ret ){
		ret->Identifier = Identifier;
		mEntities.push_back( ret );
	}
	return ret;
}

void CWorldManager::DestroyEntity(CEntity* Entity)
{
	if( !Entity ) return;
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		if( *ent == Entity ){
			delete *ent;
			mEntities.erase(ent);
			break;
		}
	}
	return;
}

CEntity* CWorldManager::GetEntityFromIdentifier(unsigned int Identifier)
{
	if( !Identifier ) return 0;
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		if( (*ent)->Identifier == Identifier ) return *ent;
	}
	return 0;
}