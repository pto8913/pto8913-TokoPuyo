#pragma once

#include "Framework/GameModeBase.h"

class GameMode_Dungeon : public GameModeBase
{
public:
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameMode_Dungeon_" + std::to_string(mID);
	}
};