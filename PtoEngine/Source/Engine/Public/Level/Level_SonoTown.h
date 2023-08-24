#pragma once

#include "Level2D.h"

class DirectX11;

class SkySphere;

class Level_SonoTown : public Level2D
{
public:
	Level_SonoTown(DirectX11& dx);
	virtual ~Level_SonoTown();

	virtual void Tick(DirectX11& dx, float deltaTime) override;

protected:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;
	virtual void GenerateItemLayer() override;
	virtual void GenerateBuildingLayer() override;
	virtual void GenerateCharacterLayer() override;
	virtual void GenerateEffectLayer() override;

	std::shared_ptr<GroundBase> G(const EGroundTile& id);

	std::shared_ptr<SkySphere> pSkySphere = nullptr;
};