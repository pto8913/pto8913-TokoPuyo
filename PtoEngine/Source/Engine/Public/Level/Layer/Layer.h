#pragma once

#include "GroundTypes.h"
#include "EventTypes.h"
#include "ItemTypes.h"
#include "BuildingTypes.h"
#include "CharacterTypes.h"
#include "EffectTypes.h"

class Layer
{
public:
	Layer();
	virtual ~Layer();

	EGroundId mGroundID;
	EEventId mEventID;
	EItemId mItemID;
	ECharacterId mCharacterID;
	ECharacterId mCharacter2ID;
};