#pragma once

#include "Framework/GameStateBase.h"

/*
*
*
*
*
*/
class GameState : public GameStateBase
{
public:
	GameState();
	virtual ~GameState();

	GameState(const GameState&) = delete;
	GameState& operator=(const GameState&) = delete;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		return "GameState_" + std::to_string(mID);
	}
};