
#include "World/World_Title.h"

#include "Level/LevelFactory.h"

void World_Title::SetLevel(DirectX11& dx)
{
	auto pLevelFactory = std::make_shared<LevelFactory>();
	pPersistentLevel = pLevelFactory->Create(dx, ELevelId::Title);

	World::SetLevel(dx);
}