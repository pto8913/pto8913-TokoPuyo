#pragma once

#include "LevelTypes.h"
#include <memory>

class Level;
class DirectX11;

class LevelFactory
{
public:
	std::shared_ptr<Level> Create(DirectX11& dx, const ELevelId& id);
};