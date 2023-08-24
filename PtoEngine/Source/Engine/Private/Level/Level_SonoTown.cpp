
#include "Level/Level_SonoTown.h"

#include "Engine/World.h"
#include "Core/DirectX.h"

#include "Object/Ground/GroundBase.h"

#include "Object/SkySphere.h"

Level_SonoTown::Level_SonoTown(DirectX11& dx)
	: Level2D(dx)
{
	mGroundType = EGroundType::Cave;

	pSkySphere = std::make_shared<SkySphere>(dx, 50.f);
}
Level_SonoTown::~Level_SonoTown()
{

}

void Level_SonoTown::Tick(DirectX11& dx, float deltaTime)
{
	pSkySphere->ExecuteTasks(dx);

	Level2D::Tick(dx, deltaTime);

}
void Level_SonoTown::GenerateGroundLayer()
{
	Init(20, 1);

	SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), 0, 19, 0, true);

	ShowTiles();

}
void Level_SonoTown::GenerateEventLayer()
{
	SetEventLayerID(EEventId::Enter, FVector2D(0, 0));

	SetEventLayerID(EEventId::Enter, FVector2D(5, 0));

}
void Level_SonoTown::GenerateItemLayer()
{
}
void Level_SonoTown::GenerateBuildingLayer()
{
}
void Level_SonoTown::GenerateCharacterLayer()
{
}
void Level_SonoTown::GenerateEffectLayer()
{
}

std::shared_ptr<GroundBase> Level_SonoTown::G(const EGroundTile& id)
{
	return GetWorld()->SpawnActor<GroundBase>(*pDX, ConvertToGround(id, mGroundType));
}