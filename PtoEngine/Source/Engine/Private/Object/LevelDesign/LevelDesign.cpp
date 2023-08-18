
#include "Object/LevelDesign/LevelDesign.h"

#include "GameInstance.h"

Level2DDesign::Level2DDesign(DirectX11&)
	: DX::Layer2DInterface(0), groundType(EGroundType::Plain)
{
}
Level2DDesign::~Level2DDesign()
{
	pLevel.reset();
	pLevel = nullptr;
}

void Level2DDesign::Generate(DirectX11& dx, Level2D* inLevel)
{
	pLevel = std::make_shared<Level2D>(inLevel);

	GenerateGroundLayer();
	GenerateEventLayer();
	GenerateItemLayer();
	GenerateBuildingLayer();
	GenerateCharacterLayer();
	GenerateEffectLayer();

	pLevel->Accept();
}

void Level2DDesign::SetGroundType(const EGroundType& inGroundType)
{
	groundType = inGroundType;
}