
#include "World/World_SonoTown.h"

#include "Core/DirectX.h"

#include "Level/Level_SonoTown.h"
#include "Actor/Character/Player_Dungeon.h"

World_SonoTown::World_SonoTown()
	: World_Town()
{
	mTownName = L"ƒ\ƒm‘º";
}

void World_SonoTown::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<Level_SonoTown>(dx);

	World::SetLevel(dx);
}