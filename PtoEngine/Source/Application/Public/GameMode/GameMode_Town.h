#pragma once

#include "Framework/GameModeBase.h"

class GameMode_Town : public GameModeBase
{
public:
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(GameMode_Town).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
};