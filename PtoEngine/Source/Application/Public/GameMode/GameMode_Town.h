#pragma once

#include "Framework/GameModeBase.h"

class GameMode_Town : public GameModeBase
{
public:
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameMode_Town_" + std::to_string(mID);
	}
};