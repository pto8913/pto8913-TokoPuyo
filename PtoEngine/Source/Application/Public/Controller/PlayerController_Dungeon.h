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
		using namespace std::string_literals;
		return typeid(PlayerController_Dungeon).name() + std::string("_") + std::to_string(mID);
	}
protected:

};