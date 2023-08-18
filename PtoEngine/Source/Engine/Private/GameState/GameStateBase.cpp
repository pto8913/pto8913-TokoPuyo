
#include "GameState/GameStateBase.h"

#include "Core/DirectX.h"
#include "Engine/World.h"

#include "GameInstance.h"

GameStateBase::GameStateBase()
{

}

GameStateBase::~GameStateBase()
{

}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void GameStateBase::BeginPlay(DirectX11& dx)
{
	GenerateWorld(dx);
}

void GameStateBase::EndPlay()
{

}

void GameStateBase::Tick(DirectX11& dx, float deltaTime)
{

}

void GameStateBase::RestartGame(DirectX11& dx)
{
	GenerateWorld(dx);

	GameInstance& gameInstance = GameInstance::Get();
	gameInstance.GetWorld()->Accept();
}