#pragma once

#include "GameState_Play.h"

class GameState_TokoPuyo : public GameState_Play
{
public:
	GameState_TokoPuyo();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(GameState_TokoPuyo).name() + std::string("_") + std::to_string(mID);
	}
};