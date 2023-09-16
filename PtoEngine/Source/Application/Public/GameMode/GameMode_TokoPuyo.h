#pragma once

#include "GameMode.h"

class GameMode_TokoPuyo : public GameMode
{
public:
	GameMode_TokoPuyo();

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "GameMode_TokoPuyo" + "#"s + std::to_string(mID);
	}
};