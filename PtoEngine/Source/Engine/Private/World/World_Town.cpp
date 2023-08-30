
#include "World/World_Town.h"

#include "Core/DirectX.h"

#include "Controller/PlayerController_Town.h"
#include "GameMode/GameMode_Town.h"
#include "GameState/GameState_Town.h"
#include "Object/Character/Player_Town.h"

#include "UI/HUD.h"

World_Town::World_Town()
	: World()
{
}

void World_Town::SetGameMode(DirectX11& dx)
{
	pGameMode = std::make_shared<GameMode_Town>();

	World::SetGameMode(dx);
}

void World_Town::SetGameState(DirectX11& dx)
{
	pGameState = std::make_shared<GameState_Town>();
	auto gameState = static_pointer_cast<GameState_Town>(pGameState);
	gameState->SetTownName(mTownName);

	World::SetGameState(dx);
}

void World_Town::SetPlayerController(DirectX11& dx)
{
	pPlayerController = std::make_shared<PlayerController_Town>(dx);

	World::SetPlayerController(dx);
}

void World_Town::SetPlayer(DirectX11& dx)
{
	pPlayer = SpawnActor<Player_Town>(dx);

	World::SetPlayer(dx);
}

void World_Town::SetHUD(DirectX11& dx)
{
	World::SetHUD(dx);

	pPlayer->OnPlayerMoved.Bind<&HUD::PlayerMoved>(*pHUD.get(), "HUD");
}
