
#include "World/World_SonoCave.h"

#include "Core/DirectX.h"

#include "Actor/Character/Player_Dungeon.h"
#include "UI/HUD.h"

#include "Level/LevelFactory.h"
#include "Level/MazeGenerator.h"

World_SonoCave::World_SonoCave()
	: World_Dungeon()
{
	mDungeonName = L"É\ÉmçzéR";
}

void World_SonoCave::SetLevel(DirectX11& dx)
{
	auto pLevelFactory = std::make_shared<LevelFactory>();
	pPersistentLevel = pLevelFactory->Create(dx, ELevelId::SonoCave);

	World::SetLevel(dx);
}

void World_SonoCave::SetHUD(DirectX11& dx)
{
	World_Dungeon::SetHUD(dx);

	auto maze = static_pointer_cast<MazeGenerator>(pPersistentLevel);
	maze->GetDungeonNextFloor().Bind<&HUD::NextFloor>(*pHUD.get(), "HUD");
	maze->GetEnterBlock().Bind<&Player_Dungeon::EnterDungeonBlock>(*static_pointer_cast<Player_Dungeon>(pPlayer).get(), "Player");
}
