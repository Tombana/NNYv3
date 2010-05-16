#pragma once

#include "Entity.h"
#include "Item.h"
#include "Npc.h"
#include "CombatEntity.h"
#include "Player.h"
#include "Monster.h"
#include "LocalPlayer.h"
#include <list>

class CWorldManager
{
public:
	CWorldManager(void);
	~CWorldManager(void);

	//These functions will create the entities in memory.
	//They will not actually put a real item on the ground that can be picked up.
	CItem* CreateItem(unsigned int Identifier);
	CPlayer* CreatePlayer(unsigned int Identifier);
	CMonster* CreateMonster(unsigned int Identifier);
	CNpc* CreateNPC(unsigned int Identifier);

	void DestroyEntity(CEntity* ent);

	CEntity* GetEntityFromIdentifier(unsigned int Identifier);

private:
	//TODO: Should this be optimized to a binary-searchable list?
	//TODO: Should the entity list have mutex protection?
	typedef std::list<CEntity*> EntityList;
	EntityList	mEntities;
	CEntity* CreateEntity(EntityType Type, unsigned int Identifier);
};
