
#include "Framework/World.h"

#include "Core/DirectX.h"

#include "Framework/GameModeBase.h"
#include "Framework/GameStateBase.h"
#include "Framework/PlayerController.h"
#include "Framework/Level/Level.h"

#include "Object/Actor.h"
#include "UI/UserWidget.h"

#include "EngineSettings.h"

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
World::World()
{
}
World::~World()
{
	OnGameModeChanged.ClearBind();
	OnPlayerControllerChanged.ClearBind();

	mTimerManager.Clear();

	pHUD.reset();
	pHUD = nullptr;
	
	pPlayer.reset();
	pPlayer = nullptr;

	pGameMode.reset();
	pGameMode = nullptr;

	pGameState.reset();
	pGameState = nullptr;

	pPersistentLevel.reset();
	pPersistentLevel = nullptr;

	pCachedPersistentLevel.reset();
	pCachedPersistentLevel = nullptr;

	pPlayerController.reset();
	pPlayerController = nullptr;
}

// -----------------------------------
// Initialize
// -----------------------------------
void World::Init(DirectX11& dx)
{
	SetLevel(dx);
	SetGameMode(dx);
	SetGameState(dx);
	SetPlayerController(dx);
	SetPlayer(dx);
	SetHUD(dx);
}
void World::SetLevel(DirectX11& dx)
{
	if (pPersistentLevel == nullptr)
	{
		pPersistentLevel = std::make_shared<Level>(dx);
	}
	pPersistentLevel->SetWorld(this);
}
void World::SetGameMode(DirectX11& dx)
{
	if (pGameMode == nullptr)
	{
		pGameMode = std::make_shared<GameModeBase>();
	}
	pGameMode->SetOuter(pPersistentLevel.get());
}
void World::SetGameState(DirectX11& dx)
{
	if (pGameState == nullptr)
	{
		pGameState = std::make_shared<GameStateBase>();
	}
	pGameState->SetOuter(pPersistentLevel.get());
}
void World::SetPlayerController(DirectX11& dx)
{
	if (pPlayerController == nullptr)
	{
		pPlayerController = std::make_shared<PlayerController>(dx);
	}
	pPlayerController->SetOuter(pPersistentLevel.get());
}
void World::SetPlayer(DirectX11& dx)
{
	if (pPlayer == nullptr)
	{
		pPlayer = SpawnActor<Actor>();
	}
	pPlayer->SetOuter(pPersistentLevel.get());
	pPlayer->SetActorLocation(pPersistentLevel->GetStartPosition());
}
void World::SetHUD(DirectX11& dx)
{
	if (pHUD == nullptr)
	{
		const auto windowSize = EngineSettings::GetWindowSize();
		pHUD = std::make_shared<UserWidget>(
			this,
			dx,
			GetPlayerController()->GetMouse(),
			windowSize.x,
			windowSize.y
		);
	}
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void World::BeginPlay(DirectX11& dx)
{
	pPersistentLevel->BeginPlay(dx);
	pGameMode->BeginPlay(dx);
	pGameState->BeginPlay(dx);
	pPlayerController->BeginPlay(dx);
	pPlayer->BeginPlay(dx);
	pHUD->AddToViewport();
}

void World::Tick(DirectX11& dx, float deltaSec)
{
	mWorldDelta = deltaSec;

	if (mTimerManager)
	{
		mTimerManager.Tick();
	}

	if (pGameMode != nullptr)
	{
		if (pGameMode->GetTickEnabled())
		{
			pGameMode->Tick(dx, deltaSec);
		}
	}

	if (pGameState != nullptr)
	{
		if (pGameState->GetTickEnabled())
		{
			pGameState->Tick(dx, deltaSec);
		}
	}

	if (pPlayerController != nullptr)
	{
		if (pPlayerController->GetTickEnabled())
		{
			pPlayerController->Tick(dx, deltaSec);
		}
	}

	if (pPersistentLevel != nullptr)
	{
		if (pPersistentLevel->GetTickEnabled())
		{
			pPersistentLevel->Tick(dx, deltaSec);
		}
	}

	if (pPlayer != nullptr)
	{
		if (pPlayer->GetTickEnabled())
		{
			pPlayer->Tick(dx, deltaSec);
		}
	}

	if (pHUD != nullptr)
	{
		if (pHUD->GetTickEnabled())
		{
			pHUD->Tick(dx, deltaSec);
		}
	}
}

// -----------------------------------
// Main : Util
// -----------------------------------
World* World::GetWorld()
{
	return this;
}
TimerManager& World::GetTimerManager()
{
	return mTimerManager;
}

// -----------------------------------
// Main : Util : Level
// -----------------------------------
Level* World::GetLevel()
{
	return pPersistentLevel.get();
}
void World::ActivateLevel()
{
	pPersistentLevel->SetTickEnabled(true);
}
void World::DeactivateLevel()
{
	pPersistentLevel->SetTickEnabled(false);
}
void World::OpenSubLevel(DirectX11& dx, std::shared_ptr<Level> inNewLevel)
{
	if (pCachedPersistentLevel != nullptr)
	{
		pPersistentLevel = std::move(inNewLevel);
	}
	else
	{
		pCachedPersistentLevel = std::move(pPersistentLevel);
		pPersistentLevel = std::move(inNewLevel);
	}
	pPersistentLevel->BeginPlay(dx);
}
void World::CloseSubLevel(DirectX11& dx)
{
	pPersistentLevel.reset();
	pPersistentLevel = nullptr;

	pPersistentLevel = std::move(pCachedPersistentLevel);
	pPersistentLevel->BeginPlay(dx);
}

// -----------------------------------
// Main : Util : GameMode
// -----------------------------------
GameModeBase* World::GetGameMode()
{
	return pGameMode.get();
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

	OnGameModeChanged.Broadcast(pGameMode.get());
}

// -----------------------------------
// Main : Util : GameState
// -----------------------------------
GameStateBase* World::GetGameState()
{
	return pGameState.get();
}
PlayerController* World::GetPlayerController()
{
	return pPlayerController.get();
}

// -----------------------------------
// Main : Util : Player
// -----------------------------------
Actor* World::GetPlayer()
{
	return pPlayer.get();
}
void World::ActivatePlayer()
{
	pPlayer->SetTickEnabled(true);
}
void World::DeactivatePlayer()
{
	pPlayer->SetTickEnabled(false);
}

// -----------------------------------
// Main : Util : HUD
// -----------------------------------
UserWidget* World::GetHUD()
{
	return pHUD.get();
}
void World::ActivateHUD()
{
	pHUD->SetTickEnabled(true);
}
void World::DeactivateHUD()
{
	pHUD->SetTickEnabled(false);
}

float World::GetWorldDeltaSec() const noexcept
{
	return mWorldDelta;
}