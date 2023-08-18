
#include "Engine/World.h"
#include "Core/DirectX.h"

#include "GameMode/GameModeBase.h"
#include "GameState/GameStateBase.h"
#include "Controller/PlayerController.h"
#include "Level/Level.h"

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
World::World()
{
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

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void World::Init(DirectX11& dx)
{
	SetLevel(dx);
	SetPlayerController(dx);
	SetGameMode(dx);
	SetGameState(dx);
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
	mWorldDelta = deltaSec;

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

std::shared_ptr<World> World::GetWorld()
{
	return shared_from_this();
}

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
	pGameMode->SetOuter(shared_from_this());
}
void World::SetGameState(DirectX11& dx)
{
	if (pGameState == nullptr)
	{
		pGameState = std::make_shared<GameStateBase>();
	}
	pGameState->SetOuter(shared_from_this());
}
void World::SetPlayerController(DirectX11& dx)
{
	if (pPlayerController == nullptr)
	{
		pPlayerController = std::make_shared<PlayerController>(dx);
	}
	pPlayerController->SetOuter(shared_from_this());
}
void World::SetLevel(DirectX11& dx)
{
	if (pPersistentLevel == nullptr)
	{
		pPersistentLevel = std::make_shared<Level>(dx);
	}
	pPersistentLevel->SetWorld(shared_from_this());
}

// -----------------------------------
// Main : Util
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

float World::GetWorldDeltaSec() const noexcept
{
	return mWorldDelta;
}