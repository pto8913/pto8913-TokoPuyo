
#include "Level/Level_SonoCave.h"

#include "Controller/PlayerController_Dungeon.h"
#include "Core/DirectX.h"

#include "Object/LevelDesign/MazeGenerator.h"

#include "GameInstance.h"

Level_SonoCave::Level_SonoCave(DirectX11& dx)
	: Level2D(dx)
{

}

Level_SonoCave::~Level_SonoCave()
{
	pMazeGenerator.reset();
	pMazeGenerator = nullptr;
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
void Level_SonoCave::NextFloor(DirectX11& dx)
{
	++mFloorCount;

	GameInstance& gameInstance = GameInstance::Get();
	//gameInstance.GetHUD().Effect_Blackout();

	GenerateWorld(dx);
}
void Level_SonoCave::GenerateWorld(DirectX11& dx)
{
	if (pMazeGenerator == nullptr)
	{
		pMazeGenerator = std::make_shared<MazeGenerator>(dx);
	}
	pMazeGenerator->SetGroundType(mDungeonType);
	pMazeGenerator->Generate(dx, this);
}
