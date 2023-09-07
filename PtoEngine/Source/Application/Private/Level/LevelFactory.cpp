
#include "Level/LevelFactory.h"

#include "Core/DirectX.h"

#include "Level/Level_TokoPuyo.h"

std::shared_ptr<Level> LevelFactory::Create(DirectX11& dx, const ELevelId& id)
{
	switch (id)
	{
	case ELevelId::TokoPuyo:
		return std::make_shared<Level_TokoPuyo>(dx);
	default:
		return nullptr;
	}
}
