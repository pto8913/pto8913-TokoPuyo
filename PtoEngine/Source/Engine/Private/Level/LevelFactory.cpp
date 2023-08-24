
#include "Level/LevelFactory.h"
#include "Level/Level.h"

#include "Core/DirectX.h"

#include "Level/Level_SonoTown.h"
#include "Level/MazeGenerator.h"

std::shared_ptr<Level> LevelFactory::Create(DirectX11& dx, const ELevelId& id)
{
	switch (id)
	{
	case ELevelId::SonoTown:
		return std::make_shared<Level_SonoTown>(dx);
	case ELevelId::SonoCave:
		return std::make_shared<MazeGenerator>(dx);
	default:
		return nullptr;
	}
}
