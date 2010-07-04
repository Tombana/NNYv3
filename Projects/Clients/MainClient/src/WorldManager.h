#pragma once

#include "Entity.h"
#include "Item.h"
#include "Npc.h"
#include "CombatEntity.h"
#include "Player.h"
#include "Monster.h"
#include "LocalPlayer.h"
#include <map>

class CWorldManager
{
public:
	CWorldManager(void);
	~CWorldManager(void);

	//Delete all entities
	void Cleanup(void);

	CEntity* GetEntityFromIdentifier(unsigned int Identifier);

	//TODO: Should the entity list have mutex protection?
	typedef std::multimap<unsigned int,CEntity*> EntityList;
	EntityList	mEntities;
	CLocalPlayer* LocalPlayer;

private:
	friend class CEntity;
	//Called by CEntity::CEntity (constructor)
	void RegisterEntity(unsigned int Identifier, CEntity* Entity);
	//Called by CEntity::~CEntity (deconstructor)
	void UnregisterEntity(CEntity* Entity);
	//Called by CEntity::SetIdentifier
	void ChangeIdentifier(CEntity* Entity, unsigned int NewIdentifier);

	CEntity* CreateEntity(EntityType Type, unsigned int Identifier, Ogre::SceneNode *Node);
};
