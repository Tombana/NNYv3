#pragma once
#include "entity.h"

class CCombatEntity :
	public CEntity
{
public:
	CCombatEntity( EntityType Type );
	virtual ~CCombatEntity(void);
};
