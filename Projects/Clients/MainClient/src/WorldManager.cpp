#include "WorldManager.h"

CWorldManager::CWorldManager(void) : mEntities(), LocalPlayer(0)
{
}

CWorldManager::~CWorldManager(void)
{
	Cleanup();
}

void CWorldManager::Cleanup(void)
{
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		delete ent->second;
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

CLocalPlayer* CWorldManager::CreateLocalPlayer(Ogre::SceneNode* Node)
{
	if( LocalPlayer != 0 ) return LocalPlayer; //Already exists
	LocalPlayer = new CLocalPlayer(Node);
	//mEntities.push_back( LocalPlayer );
	//TODO: Find a solution: there is no known identifier yet so how to add it to the list
	mEntities[-1] = LocalPlayer;
	return LocalPlayer;
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
		mEntities[Identifier] = ret;
	}
	return ret;
}

void CWorldManager::DestroyEntity(unsigned int Identifier)
{
	if( !Identifier ) return;
	EntityList::iterator ent = mEntities.find(Identifier);
	if( ent != mEntities.end() ){
		delete ent->second;
		mEntities.erase(ent);
	}
	return;
}

void CWorldManager::DestroyEntity(CEntity* Entity)
{
	if( !Entity ) return;
	//DestroyEntity(Entity->Identifier);
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		if( ent->second == Entity ){
			mEntities.erase(ent);
			break;
		}
	}
	delete Entity;
	return;
}

CEntity* CWorldManager::GetEntityFromIdentifier(unsigned int Identifier)
{
	if( !Identifier ) return 0;
	const EntityList::iterator entity = mEntities.find(Identifier);
	if( entity != mEntities.end() ) return entity->second;
	return 0;
}