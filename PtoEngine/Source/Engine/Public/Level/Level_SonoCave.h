#pragma once

#include "Level.h"

class MazeGenerator;

class Level_SonoCave : public Level2D
{
public:
	Level_SonoCave(DirectX11& dx);
	virtual ~Level_SonoCave();

public:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual void GenerateWorld(DirectX11& dx) override;
	virtual void NextFloor(DirectX11& dx);

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	int mFloorCount = 0;
	EGroundType mDungeonType = EGroundType::Cave;

	std::shared_ptr<MazeGenerator> pMazeGenerator = nullptr;
};