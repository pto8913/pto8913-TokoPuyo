#pragma once

#include "DirectX/DirectXHead.h"

#include "LevelDesign.h"

class Level_SonoTown : public Level2DDesign
{
public:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;

private:

};