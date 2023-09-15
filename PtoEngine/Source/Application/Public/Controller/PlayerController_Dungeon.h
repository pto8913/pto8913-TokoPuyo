#pragma once

#include "Framework/PlayerController.h"

class DirectX11;

class PlayerController_Dungeon : public PlayerController
{
public:
	PlayerController_Dungeon(DirectX11& dx);
	~PlayerController_Dungeon();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual std::string GetName() const override
	{
		return "PlayerController_Dungeon_" + std::to_string(mID);
	}
protected:

};