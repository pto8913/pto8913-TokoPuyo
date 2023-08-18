#pragma once

#include "Level/Level.h"
#include <memory>

#include "Object/LayerInterface.h"

class DirectX11;

class Level2DDesign : public DX::Layer2DInterface
{
public:
	Level2DDesign(DirectX11& dx);
	virtual ~Level2DDesign();

	void Generate(DirectX11& dx, Level2D* inLevel);
	void SetGroundType(const EGroundType& inGroundType);

protected:
	virtual void GenerateGroundLayer() = 0;
	virtual void GenerateEventLayer() = 0;
	virtual void GenerateItemLayer() {}
	virtual void GenerateBuildingLayer() {}
	virtual void GenerateCharacterLayer() {}
	virtual void GenerateEffectLayer() {}
protected:
	EGroundType groundType;
	std::shared_ptr<Level2D> pLevel = nullptr;
};