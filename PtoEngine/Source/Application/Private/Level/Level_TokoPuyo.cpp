
#include "Level/Level_TokoPuyo.h"

#include "Framework/World.h"
#include "Core/DirectX.h"

#include "Actor/Ground/GroundBase.h"

#include "Math/Math.h"

#include "Actor/Event/EventBase.h"

Level_TokoPuyo::Level_TokoPuyo(DirectX11& dx)
	: Level2D(dx)
{
	mGroundType = EGroundType::Cave;
}
Level_TokoPuyo::~Level_TokoPuyo()
{

}

void Level_TokoPuyo::BeginPlay(DirectX11& dx)
{
	Level2D::BeginPlay(dx);

	GetWorld()->GetHUD();

	static_pointer_cast<>(GetWorld()->GetPlayer());

}
void Level_TokoPuyo::Tick(DirectX11& dx, float deltaTime)
{
	Level2D::Tick(dx, deltaTime);

	pEventBase->Tick(dx, deltaTime);
}
void Level_TokoPuyo::GenerateGroundLayer()
{
	Init(20, 1);

	SetGroundLayerID(ConvertToGround(EGroundTile::Path, mGroundType), 0, 19, 0, true);

	ShowTiles();

	pEventBase = GetWorld()->SpawnActor<EventBase>(*pDX, EEventId::Block);
	pEventBase->BeginPlay(*pDX);
	pEventBase->SetActorLocation(FVector(1, 0, 0));
}
void Level_TokoPuyo::GenerateEventLayer()
{
	SetEventLayerID(EEventId::Enter, 0, 0);

	SetEventLayerID(EEventId::Enter, 5, 0);
}
void Level_TokoPuyo::GenerateItemLayer()
{
}
void Level_TokoPuyo::GenerateBuildingLayer()
{
}
void Level_TokoPuyo::GenerateCharacterLayer()
{
}
void Level_TokoPuyo::GenerateEffectLayer()
{
}
