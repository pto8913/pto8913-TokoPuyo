
#include "World/World_SonoCave.h"

#include "Core/DirectX.h"

#include "Level/MazeGenerator.h"
#include "Controller/PlayerController_Dungeon.h"
#include "GameMode/GameMode_Dungeon.h"
#include "GameState/GameState_Dungeon.h"

World_SonoCave::World_SonoCave()
	: World()
{

}
World_SonoCave::~World_SonoCave()
{

}

void World_SonoCave::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<MazeGenerator>(dx);

	World::SetLevel(dx);
}

void World_SonoCave::SetPlayerController(DirectX11& dx)
{

	pPlayerController = SpawnActor<PlayerController_Dungeon>(dx);

	World::SetPlayerController(dx);
}

void World_SonoCave::SetGameMode(DirectX11& dx)
{
	pGameMode = SpawnActor<GameMode_Dungeon>();

	World::SetGameMode(dx);
}

void World_SonoCave::SetGameState(DirectX11& dx)
{
	pGameState = SpawnActor<GameState_Dungeon>();

	World::SetGameState(dx);
}