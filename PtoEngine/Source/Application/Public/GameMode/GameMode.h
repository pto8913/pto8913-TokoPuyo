#pragma once

#include "Framework/GameModeBase.h"

class GameMode : public GameModeBase
{
public:
	GameMode();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(GameMode).name() + std::string("_") + std::to_string(mID);
	}
};