
#include "GameInstance.h"

GameInstance::~GameInstance()
{
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameInstance::Initialize(DirectX11& dx)
{
}

// -----------------------------------
// Main : Utils
// -----------------------------------
GameInstance& GameInstance::Get()
{
	static GameInstance instance;
	return instance;
}
