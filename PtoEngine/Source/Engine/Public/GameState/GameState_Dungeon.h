#pragma once

#include "GameStateBase.h"

#include <string>

class MazeGenerator;

class GameState_Dungeon : public GameStateBase
{
public:
	GameState_Dungeon();
	virtual ~GameState_Dungeon();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetDungeonName(const std::wstring& in);
	std::wstring GetDungeonName() const noexcept;

	void IncreaseDungeonFloor();
	int GetDungeonFloor() const noexcept;

	std::wstring GetDungeonFloorName() const noexcept;
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::wstring mDungeonName;
	int mDungeonFloor;
};