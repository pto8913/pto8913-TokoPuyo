
#include "GameState/GameState_Dungeon.h"

#include "Object/LevelDesign/MazeGenerator.h"

#include "GameInstance.h"

GameState_Dungeon::GameState_Dungeon()
	: GameStateBase()
{

}

GameState_Dungeon::~GameState_Dungeon()
{
	pMazeGenerator.reset();
	pMazeGenerator = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void GameState_Dungeon::NextFloor(DirectX11& dx)
{
	++mFloorCount;

	GameInstance& gameInstance = GameInstance::Get();
	//gameInstance.GetHUD().Effect_Blackout();

	GenerateWorld(dx);
}
void GameState_Dungeon::GenerateWorld(DirectX11& dx)
{
	if (pMazeGenerator == nullptr)
	{
		pMazeGenerator = std::make_shared<MazeGenerator>(dx);
	}
	pMazeGenerator->SetGroundType(mDungeonType);
	pMazeGenerator->Generate(dx);
}
