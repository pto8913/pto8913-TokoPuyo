
#include "Level/Level_SonoTown.h"

#include "Engine/World.h"
#include "Core/DirectX.h"

#include "Object/Ground/GroundBase.h"

#include "Object/DrawPlane.h"
#include "Object/SkyLight.h"

#include "Math/Math.h"

#include "Object/Event/EventBase.h"

Level_SonoTown::Level_SonoTown(DirectX11& dx)
	: Level2D(dx)
{
	mGroundType = EGroundType::Cave;

	pSkyLight = std::make_shared<SkyLight>(dx);
	pDrawPlane = std::make_shared<DrawPlane>(dx, 1.f, 1.f, L"Content/Textures/T_BG_SonoTown.png");
	pDrawPlane->SetRotation({ -PI / 2.f, 0, 0 });
	pDrawPlane->SetLocation({ 0, 0, 0 });
}
Level_SonoTown::~Level_SonoTown()
{

}

void Level_SonoTown::Tick(DirectX11& dx, float deltaTime)
{
	pSkyLight->ExecuteTasks(dx);
	pDrawPlane->ExecuteTasks(dx);

	//Level2D::Tick(dx, deltaTime);

	pEventBase->Tick(dx, deltaTime);
}
void Level_SonoTown::GenerateGroundLayer()
{
	Init(20, 1);

	SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), 0, 19, 0, true);

	ShowTiles();

	pEventBase = GetWorld()->SpawnActor<EventBase>(*pDX, EEventId::Block);
	pEventBase->SetActorLocation(FVector(1, 0, 0));
	pEventBase->SetOffset(WorldToScreen(1, 0, pEventBase->GetActorScale().To2D()));
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