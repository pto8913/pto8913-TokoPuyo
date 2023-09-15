#pragma once

#include "GameState.h"

#include <string>

class MazeGenerator;

class GameState_Town : public GameState
{
public:
	GameState_Town();
	virtual ~GameState_Town();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		return "GameState_Town_" + std::to_string(mID);
	}

	void SetTownName(const std::wstring& in);
	std::wstring GetTownName() const noexcept;

private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::wstring mTownName;
};