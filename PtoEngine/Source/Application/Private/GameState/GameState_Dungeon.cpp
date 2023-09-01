
#include "GameState/GameState_Dungeon.h"

#include <format>

GameState_Dungeon::GameState_Dungeon()
{
	mDungeonName = L"";
	mDungeonFloor = 0;
}

GameState_Dungeon::~GameState_Dungeon()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameState_Dungeon::OpenLandmarkUI(DirectX11& dx, const std::wstring& inLandmarkName, const float& inPlayRate, const FOnWidgetAnimationCompleted& inCompleted, const float& inCompleteToDelay)
{
	IncreaseDungeonFloor();

	GameState::OpenLandmarkUI(dx, GetDungeonFloorName(), inPlayRate, inCompleted, inCompleteToDelay);
}

void GameState_Dungeon::SetDungeonName(const std::wstring& in)
{
	mDungeonName = in;
}
std::wstring GameState_Dungeon::GetDungeonName() const noexcept
{
	return mDungeonName;
}

void GameState_Dungeon::IncreaseDungeonFloor()
{
	++mDungeonFloor;
}

int GameState_Dungeon::GetDungeonFloor() const noexcept
{
	return mDungeonFloor;
}

std::wstring GameState_Dungeon::GetDungeonFloorName() const noexcept
{
	return std::format(L"{} {}F", mDungeonName, mDungeonFloor);
}