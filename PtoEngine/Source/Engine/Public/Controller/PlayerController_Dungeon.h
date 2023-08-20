#pragma once

#include "Controller/PlayerController.h"

class DirectX11;

class PlayerController_Dungeon : public PlayerController
{
public:
	PlayerController_Dungeon(DirectX11& dx, std::shared_ptr<Object> inOuter);
	~PlayerController_Dungeon();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
protected:

};