/*
* Default Layer Order :
*
* Ground Layer (e.g. ground, river, grass...etc.
* Event Layer (e.g. exit, start, carpet like shop.
* Item Layer (e.g. potion, weapon...etc.
* Building Layer (e.g. house, wood...etc.
* Character Layer (e.g. player, enemy...etc.
* Effect Layer (e.g. hit, weather...etc.
*
* If you are not sure where your working layer belongs, ask pto8913. (The only one working on it was pto8913 lol.
*
* What is Layer2D
*
* The smaller the Layer, the further back the object is displayed.
*
* ex. the player Layer is 1, the house Layer is 0,
*
* Default, the player try to overlap the house, the player appear front of the house.
*
* Next, the player press up key, the player Layer is 0.
* In the state, the player try to overlap the house, the player stop moving in side of the house.
*
* Next, the player press up key once more. the player Layer is -1.
* In the state, the player try to overlap the house, the player behind the house.
*/

#pragma once

#include "Level/Layer/GroundTypes.h"
#include "Level/Layer/EventTypes.h"
#include "Level/Layer/ItemTypes.h"
#include "Level/Layer/BuildingTypes.h"
#include "Level/Layer/CharacterTypes.h"
#include "Level/Layer/EffectTypes.h"

class LayerHelper
{
public:
	static int ConvertTypeToLayer(const EGroundId& in);
	static int ConvertTypeToLayer(const EEventId& in);
	static int ConvertTypeToLayer(const EItemId& in);
	static int ConvertTypeToLayer(const EBuildingId& in);
	static int ConvertTypeToLayer(const ECharacterId& in);
	//static int ConvertTypeToLayer(const EEffectId& in);

	static bool IsGroundId(int inLayer);
	static bool IsEventId(int inLayer);
	static bool IsItemId(int inLayer);
	static bool IsBuildingId(int inLayer);
	static bool IsCharacterId(int inLayer);
	//static bool IsGroundId(int inLayer);

	static int GetGroundId();
	static int GetEventId();
	static int GetItemId();
	static int GetBuildingId();
	static int GetCharacterId();
	//static int GetGroundId();
};