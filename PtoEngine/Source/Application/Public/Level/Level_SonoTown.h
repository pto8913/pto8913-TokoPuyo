#pragma once

#include "Level2D.h"

class DirectX11;

class DrawPlane;
class SkyLight;

class Level_SonoTown : public Level2D
{
public:
	Level_SonoTown(DirectX11& dx);
	virtual ~Level_SonoTown();

	virtual void Tick(DirectX11& dx, float deltaTime) override;
	virtual std::string GetName() const override
	{
		return "Level_SonoTown_" + std::to_string(mID);
	}
protected:
	virtual void GenerateGroundLayer() override;
	virtual void GenerateEventLayer() override;
	virtual void GenerateItemLayer() override;
	virtual void GenerateBuildingLayer() override;
	virtual void GenerateCharacterLayer() override;
	virtual void GenerateEffectLayer() override;

	std::shared_ptr<SkyLight> pSkyLight = nullptr;
	std::shared_ptr<DrawPlane> pDrawPlane = nullptr;

	std::shared_ptr<class EventBase> pEventBase = nullptr;

};