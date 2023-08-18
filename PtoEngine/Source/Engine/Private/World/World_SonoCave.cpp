
#include "World/World_SonoCave.h"

#include "GameState/GameState_Dungeon.h"

#include "Controller/PlayerController_Dungeon.h"
#include "Core/DirectX.h"

#include "Object/LevelDesign/MazeGenerator.h"

World_SonoCave::World_SonoCave(DirectX11& dx)
	: World(dx)
{

}
World_SonoCave::~World_SonoCave()
{

}

void World_SonoCave::SetGameState(DirectX11& dx)
{
	pGameState = std::make_shared<GameState_Dungeon>();
}

void World_SonoCave::SetPlayerController(DirectX11& dx)
{
	pPlayerController = std::make_shared<PlayerController_Dungeon>(dx);
}

void World_SonoCave::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<MazeGenerator>(dx);
}