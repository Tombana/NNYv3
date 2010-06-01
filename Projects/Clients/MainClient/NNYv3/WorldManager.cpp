#include "WorldManager.h"

CWorldManager::CWorldManager(void) : mEntities()
{
}

CWorldManager::~CWorldManager(void)
{
}

CItem* CWorldManager::CreateItem(unsigned int Identifier)
{
	return (CItem*)CreateEntity(EntityType_Item, Identifier);
}

CPlayer* CWorldManager::CreatePlayer(unsigned int Identifier)
{
	return (CPlayer*)CreateEntity(EntityType_Player, Identifier);
}

CMonster* CWorldManager::CreateMonster(unsigned int Identifier)
{
	return (CMonster*)CreateEntity(EntityType_Monster, Identifier);
}

CNpc* CWorldManager::CreateNPC(unsigned int Identifier)
{
	return (CNpc*)CreateEntity(EntityType_NPC, Identifier);
}

CEntity* CWorldManager::CreateEntity(EntityType Type, unsigned int Identifier)
{
	//Check if it already exists
	if( GetEntityFromIdentifier(Identifier) ) return 0;
	CEntity* ret = 0;
	switch(Type){
		case EntityType_Item:
			ret = new CItem();
			break;
		case EntityType_Player:
			ret = new CPlayer();
			break;
		case EntityType_Monster:
			ret = new CMonster();
			break;
		case EntityType_NPC:
			ret = new CNpc();
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