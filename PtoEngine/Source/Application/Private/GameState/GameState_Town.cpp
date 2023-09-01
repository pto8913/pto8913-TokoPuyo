
#include "GameState/GameState_Town.h"

#include <format>

GameState_Town::GameState_Town()
{
	mTownName = L"";
}

GameState_Town::~GameState_Town()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameState_Town::SetTownName(const std::wstring& in)
{
	mTownName = in;
}
std::wstring GameState_Town::GetTownName() const noexcept
{
	return mTownName;
}
