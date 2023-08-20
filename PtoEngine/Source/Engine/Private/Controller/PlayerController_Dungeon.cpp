
#include "Controller/PlayerController_Dungeon.h"

#include "Object/Character/Player.h"

#include "Core/DirectX.h"

PlayerController_Dungeon::PlayerController_Dungeon(DirectX11& dx, std::shared_ptr<Object> inOuter)
	: PlayerController(dx, inOuter)
{

}
PlayerController_Dungeon::~PlayerController_Dungeon()
{

}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
