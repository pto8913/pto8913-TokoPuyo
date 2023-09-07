
#include "World/World_TokoPuyo.h"

#include "Core/DirectX.h"

#include "Actor/Character/Player_Dungeon.h"
#include "UI/HUD.h"

#include "Controller/PlayerController_TokoPuyo.h"
#include "GameMode/GameMode_TokoPuyo.h"
#include "GameState/GameState_TokoPuyo.h"

#include "Level/LevelFactory.h"

World_TokoPuyo::World_TokoPuyo()
	: World()
{
}

void World_TokoPuyo::SetLevel(DirectX11& dx)
{
	auto pLevelFactory = std::make_shared<LevelFactory>();
	pPersistentLevel = pLevelFactory->Create(dx, ELevelId::TokoPuyo);

	World::SetLevel(dx);
}
void World_TokoPuyo::SetGameMode(DirectX11& dx)
{
	pGameMode = std::make_shared<GameMode_TokoPuyo>();

	World::SetGameMode(dx);
}
void World_TokoPuyo::SetGameState(DirectX11& dx)
{
	pGameState = std::make_shared<GameState_TokoPuyo>();

	World::SetGameState(dx);
}
void World_TokoPuyo::SetPlayerController(DirectX11& dx)
{
	pPlayerController = std::make_shared<PlayerController_TokoPuyo>(dx);

	World::SetPlayerController(dx);
}

void World_TokoPuyo::SetHUD(DirectX11& dx)
{
	pHUD = std::make_shared<HUD>(
		shared_from_this(),
		dx,
		pPlayerController->GetMouse()
	);

	World::SetHUD(dx);
}
