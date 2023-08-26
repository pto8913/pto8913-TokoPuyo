
#include "World/World_Dungeon.h"

#include "Core/DirectX.h"

#include "Controller/PlayerController_Dungeon.h"
#include "GameMode/GameMode_Dungeon.h"
#include "GameState/GameState_Dungeon.h"
#include "Object/Character/Player_Dungeon.h"
#include "UI/HUD.h"

World_Dungeon::World_Dungeon()
	: World()
{
}

void World_Dungeon::SetGameMode(DirectX11& dx)
{
	pGameMode = std::make_shared<GameMode_Dungeon>();

	World::SetGameMode(dx);
}

void World_Dungeon::SetGameState(DirectX11& dx)
{
	pGameState = std::make_shared<GameState_Dungeon>();
	auto gameState = static_pointer_cast<GameState_Dungeon>(pGameState);
	gameState->SetDungeonName(mDungeonName);

	World::SetGameState(dx);
}

void World_Dungeon::SetPlayerController(DirectX11& dx)
{
	pPlayerController = std::make_shared<PlayerController_Dungeon>(dx);

	World::SetPlayerController(dx);
}

void World_Dungeon::SetPlayer(DirectX11& dx)
{
	pPlayer = SpawnActor<Player_Dungeon>(dx);

	World::SetPlayer(dx);
}

void World_Dungeon::SetHUD(DirectX11& dx)
{
	World::SetHUD(dx);

	pPlayer->OnPlayerMoved.Bind<&HUD::PlayerMoved>(*pHUD.get(), "HUD");
}
