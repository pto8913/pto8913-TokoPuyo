#pragma once

#include "Object/Actor.h"

class GameModeBase : public Actor
{
public:
	GameModeBase();
	virtual ~GameModeBase();

	GameModeBase(const GameModeBase&) = delete;
	GameModeBase& operator=(const GameModeBase&) = delete;

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameModeBase" + "#"s + std::to_string(mID);
	}
};