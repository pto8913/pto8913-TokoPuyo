#pragma once

#include "GameState.h"

#include <string>

class MazeGenerator;

class GameState_Dungeon : public GameState
{
public:
	GameState_Dungeon();
	virtual ~GameState_Dungeon();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		return "GameState_Dungeon_" + std::to_string(mID);
	}
	virtual void OpenLandmarkUI(DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted = {}, const float& inCompleteToDelay = 0.01f) override;

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