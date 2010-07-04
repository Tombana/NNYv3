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
	//Calling the deconstructor on every entity should delete them from the list
	while( !mEntities.empty() ) delete (mEntities.begin()->second);
}

void CWorldManager::RegisterEntity(unsigned int Identifier, CEntity* Entity)
{
	mEntities.insert(EntityList::value_type(Identifier, Entity));
}

void CWorldManager::UnregisterEntity(CEntity* Entity)
{
	for( EntityList::iterator ent = mEntities.begin(); ent != mEntities.end(); ++ent ){
		if( ent->second == Entity ){
			mEntities.erase(ent);
			break;
		}
	}
}

void CWorldManager::ChangeIdentifier(CEntity* Entity, unsigned int NewIdentifier)
{
	UnregisterEntity(Entity);
	RegisterEntity(NewIdentifier, Entity);
}

CEntity* CWorldManager::GetEntityFromIdentifier(unsigned int Identifier)
{
	if( !Identifier ) return 0;
	const EntityList::iterator entity = mEntities.find(Identifier);
	if( entity != mEntities.end() ) return entity->second;
	return 0;
}