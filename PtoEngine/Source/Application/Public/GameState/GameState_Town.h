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
	void SetTownName(const std::wstring& in);
	std::wstring GetTownName() const noexcept;

private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::wstring mTownName;
};