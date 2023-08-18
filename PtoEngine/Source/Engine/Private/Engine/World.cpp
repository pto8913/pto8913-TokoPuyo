
#include "Engine/World.h"
#include "Core/DirectX.h"

#include "GameMode/GameModeBase.h"
#include "GameState/GameStateBase.h"
#include "Controller/PlayerController.h"
#include "Level/Level.h"

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
World::World(DirectX11& dx)
{
	SetGameMode(dx);
	SetGameState(dx);
	SetPlayerController(dx);
	SetLevel(dx);
}
World::~World()
{
	mTimerManager.Clear();

	pGameMode.reset();
	pGameMode = nullptr;

	pGameState.reset();
	pGameState = nullptr;

	pPlayerController.reset();
	pPlayerController = nullptr;

	pPersistentLevel.reset();
	pPersistentLevel = nullptr;
}

void World::BeginPlay(DirectX11& dx)
{
	pPersistentLevel->BeginPlay(dx);
	pPlayerController->BeginPlay(dx);
	pGameMode->BeginPlay(dx);
	pGameState->BeginPlay(dx);
}

void World::Tick(DirectX11& dx, float deltaSec)
{
	if (pGameMode)
	{
		if (pGameMode->GetTickEnabled())
		{
			pGameMode->Tick(dx, deltaSec);
		}
	}

	if (pGameState)
	{
		if (pGameState->GetTickEnabled())
		{
			pGameState->Tick(dx, deltaSec);
		}
	}

	if (pPlayerController)
	{
		if (pPlayerController->GetTickEnabled())
		{
			pPlayerController->Tick(dx, deltaSec);
		}
	}

	if (pPersistentLevel)
	{
		if (pPersistentLevel->GetTickEnabled())
		{
			pPersistentLevel->Tick(dx, deltaSec);
		}
	}
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
TimerManager& World::GetTimerManager()
{
	return mTimerManager;
}
void World::SetGameMode(DirectX11& dx)
{
	if (pGameMode == nullptr)
	{
		pGameMode = std::make_shared<GameModeBase>();
	}
}
void World::SetGameState(DirectX11& dx)
{
	if (pGameState == nullptr)
	{
		pGameState = std::make_shared<GameStateBase>();
	}
}
void World::SetPlayerController(DirectX11& dx)
{
	if (pPlayerController == nullptr)
	{
		pPlayerController = std::make_shared<PlayerController>();
	}
}
void World::SetLevel(DirectX11& dx)
{
	if (pPersistentLevel == nullptr)
	{
		pPersistentLevel = std::make_shared<Level>();
	}
	pPersistentLevel->SetWorld(this);
}

// -----------------------------------
// Main : GameMode
// -----------------------------------
std::shared_ptr<GameModeBase>& World::GetGameMode()
{
	return pGameMode;
}
void World::SetGameMode(DirectX11& dx, std::shared_ptr<GameModeBase>&& NewGameMode)
{
	if (pGameMode)
	{
		pGameMode->EndPlay();
		pGameMode.reset();
	}
	pGameMode = std::move(NewGameMode);
	pGameMode->BeginPlay(dx);
	
	OnGameModeChanged.Broadcast(pGameMode);
}

// -----------------------------------
// Main : Util
// -----------------------------------
std::shared_ptr<GameModeBase>& World::GetGameMode()
{
	return pGameMode;
}
std::shared_ptr<GameStateBase>& World::GetGameState()
{
	return pGameState;
}
std::shared_ptr<PlayerController>& World::GetPlayerController()
{
	return pPlayerController;
}
std::shared_ptr<Level>& World::GetLevel()
{
	return pPersistentLevel;
}