#include "WorldManager.h"

CWorldManager::CWorldManager(void) : mEntities(), LocalPlayer(0), mSelectedEntity(0)
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

//If this returns false then the entity can not be set as selected
bool CWorldManager::SetSelectedEntity(CEntity* Entity)
{
	//if( Entity == 0 ) //Trying to deselect the selected entity
	
	if( mSelectedEntity == Entity ) return false; //Already selected this entity (or no selected entity and trying to deselect)

	mSelectedEntity = Entity;
	return true;
}

CEntity* CWorldManager::GetSelectedEntity(void)
{
	return mSelectedEntity;
}
