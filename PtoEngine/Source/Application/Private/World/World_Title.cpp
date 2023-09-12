
#include "World/World_Title.h"
#include "Level/Level_Title.h"

void World_Title::SetLevel(DirectX11& dx)
{
	pPersistentLevel = std::make_shared<Level_Title>(dx);

	World::SetLevel(dx);
}