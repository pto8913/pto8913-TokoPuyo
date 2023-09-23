
#include "Level/Level_TokoPuyo.h"

#include "Framework/World.h"

#include "Actor/Ground/GroundBase.h"

#include "GameState/GameState_Play.h"
#include "GameSettings.h"

using namespace DirectX;

Level_TokoPuyo::Level_TokoPuyo(DirectX11& dx)
	: Level2D(dx)
{
	pDX = &dx;
}
Level_TokoPuyo::~Level_TokoPuyo()
{
	pGameState = nullptr;
	pDX = nullptr;
}

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
void Level_TokoPuyo::GenerateGroundLayer()
{
	auto world = GetWorld();
	for (int y = 0; y < height - 1; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			auto ground = world->SpawnActor<GroundBase>(*pDX, EGroundId::Background);
			SetSpriteLocation(ground, x, y + 1);
			ground = nullptr;
		}
	}
}

void Level_TokoPuyo::BeginPlay(DirectX11& dx)
{
	const auto size = GameSettings::GetGameBoardSize();
	Init(size.x, size.y);

	Level2D::BeginPlay(dx);

	pGameState = static_cast<GameState_Play*>(GetWorld()->GetGameState());
}
void Level_TokoPuyo::Tick(DirectX11& dx, float deltaTime)
{
	const auto gameProgress = pGameState->GetGameProgress();
	if (gameProgress != EGameProgress::GameOver)
	{
		Level2D::Tick(dx, deltaTime);
	}
}


