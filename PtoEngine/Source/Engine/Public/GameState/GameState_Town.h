#pragma once

#include "GameStateBase.h"

#include <string>

class MazeGenerator;

class GameState_Town : public GameStateBase
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