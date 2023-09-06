
#include "World/World_PuyoPuyo.h"

#include "Core/DirectX.h"

#include "Actor/Character/Player_Dungeon.h"
#include "UI/HUD.h"

#include "Level/LevelFactory.h"
#include "Level/MazeGenerator.h"

World_PuyoPuyo::World_PuyoPuyo()
	: World()
{
}

void World_PuyoPuyo::SetLevel(DirectX11& dx)
{
	auto pLevelFactory = std::make_shared<LevelFactory>();
	pPersistentLevel = pLevelFactory->Create(dx, ELevelId::TokoPuyo);

	World::SetLevel(dx);
}

void World_PuyoPuyo::SetHUD(DirectX11& dx)
{
	World::SetHUD(dx);

	auto maze = static_pointer_cast<MazeGenerator>(pPersistentLevel);
	maze->GetDungeonNextFloor().Bind<&HUD::NextFloor>(*pHUD.get(), "HUD");
	maze->GetEnterBlock().Bind<&Player_Dungeon::EnterDungeonBlock>(*static_pointer_cast<Player_Dungeon>(pPlayer).get(), "Player");
}
