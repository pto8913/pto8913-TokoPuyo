
#include "World/World_TokoPuyo.h"

#include "Graphics/Core/DirectX.h"

#include "Controller/PlayerController_TokoPuyo.h"
#include "GameMode/GameMode_TokoPuyo.h"
#include "GameState/GameState_TokoPuyo.h"
#include "Actor/Character/Player.h"

#include "Level/Level_PuyoField.h"

World_TokoPuyo::World_TokoPuyo()
	: World()
{
}
World_TokoPuyo::~World_TokoPuyo()
{
}

void World_TokoPuyo::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<Level_PuyoField>(dx);

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
void World_TokoPuyo::SetPlayer(DirectX11& dx)
{
	pPlayer = __SpawnActor<Player>();

	World::SetPlayer(dx);
}