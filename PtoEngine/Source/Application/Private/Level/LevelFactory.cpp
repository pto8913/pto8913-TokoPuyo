
#include "Level/LevelFactory.h"

#include "Core/DirectX.h"

#include "Level/Level_TokoPuyo.h"
#include "Level/Level_Title.h"

std::shared_ptr<Level>&& LevelFactory::Create(DirectX11& dx, const ELevelId& id)
{
	switch (id)
	{
	case ELevelId::Title:
		return std::move(std::make_shared<Level_Title>(dx));
	case ELevelId::TokoPuyo:
		return std::move(std::make_shared<Level_TokoPuyo>(dx));
	default:
		return nullptr;
	}
}
