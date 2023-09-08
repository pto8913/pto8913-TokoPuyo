
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

	pPersistentLevel.reset();
	pPersistentLevel = nullptr;

	pCachedPersistentLevel.reset();
	pCachedPersistentLevel = nullptr;

	pGameMode.reset();
	pGameMode = nullptr;

	pGameState.reset();
	pGameState = nullptr;

	pPlayerController.reset();
	pPlayerController = nullptr;

	pPlayer.reset();
	pPlayer = nullptr;

	pHUD.reset();
	pHUD = nullptr;
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
	pPersistentLevel->SetWorld(shared_from_this());
}
void World::SetGameMode(DirectX11& dx)
{
	if (pGameMode == nullptr)
	{
		pGameMode = std::make_shared<GameModeBase>();
	}
	pGameMode->SetOuter(pPersistentLevel);
}
void World::SetGameState(DirectX11& dx)
{
	if (pGameState == nullptr)
	{
		pGameState = std::make_shared<GameStateBase>();
	}
	pGameState->SetOuter(pPersistentLevel);
}
void World::SetPlayerController(DirectX11& dx)
{
	if (pPlayerController == nullptr)
	{
		pPlayerController = std::make_shared<PlayerController>(dx);
	}
	pPlayerController->SetOuter(pPersistentLevel);
}
void World::SetPlayer(DirectX11& dx)
{
	if (pPlayer == nullptr)
	{
		pPlayer = SpawnActor<Actor>();
	}
	pPlayer->SetOuter(pPersistentLevel);
	pPlayer->SetActorLocation(pPersistentLevel->GetStartPosition());
}
void World::SetHUD(DirectX11& dx)
{
	if (pHUD == nullptr)
	{
		const auto windowSize = EngineSettings::GetWindowSize();
		pHUD = std::make_shared<UserWidget>(
			shared_from_this(),
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
std::shared_ptr<World> World::GetWorld()
{
	return shared_from_this();
}
TimerManager& World::GetTimerManager()
{
	return mTimerManager;
}

// -----------------------------------
// Main : Util : Level
// -----------------------------------
std::shared_ptr<Level>& World::GetLevel()
{
	return pPersistentLevel;
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
// Main : Util : GameState
// -----------------------------------
std::shared_ptr<GameStateBase>& World::GetGameState()
{
	return pGameState;
}
std::shared_ptr<PlayerController>& World::GetPlayerController()
{
	return pPlayerController;
}

// -----------------------------------
// Main : Util : Player
// -----------------------------------
std::shared_ptr<Actor>& World::GetPlayer()
{
	return pPlayer;
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
std::shared_ptr<UserWidget>& World::GetHUD()
{
	return pHUD;
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