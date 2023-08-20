
#include "World/World_SonoCave.h"

#include "Core/DirectX.h"

#include "Level/MazeGenerator.h"
#include "Controller/PlayerController_Dungeon.h"
#include "GameMode/GameMode_Dungeon.h"
#include "GameState/GameState_Dungeon.h"

#include "UI/HUD.h"

World_SonoCave::World_SonoCave()
	: mDungeonName(L"É\ÉmçzéR")
{
}

void World_SonoCave::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<MazeGenerator>(dx);

	World::SetLevel(dx);
}

void World_SonoCave::SetGameMode(DirectX11& dx)
{
	pGameMode = SpawnActor<GameMode_Dungeon>();

	World::SetGameMode(dx);
}

void World_SonoCave::SetGameState(DirectX11& dx)
{
	pGameState = SpawnActor<GameState_Dungeon>();
	auto gameState = static_pointer_cast<GameState_Dungeon>(pGameState);
	gameState->SetDungeonName(mDungeonName);

	World::SetGameState(dx);
}

void World_SonoCave::SetPlayerController(DirectX11& dx)
{
	pPlayerController = SpawnActor<PlayerController_Dungeon>(dx, shared_from_this());

	World::SetPlayerController(dx);
}