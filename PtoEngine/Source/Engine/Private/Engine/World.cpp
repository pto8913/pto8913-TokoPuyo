
#include "Engine/World.h"

#include "Core/DirectX.h"
#include "Core/AppSettings.h"

#include "GameMode/GameModeBase.h"
#include "GameState/GameStateBase.h"
#include "Controller/PlayerController.h"
#include "Level/Level.h"
#include "Object/Character/Player.h"
#include "UI/HUD.h"

#include "Level/LevelFactory.h"

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
		pPlayer = std::make_shared<Player>(dx);
	}
	pPlayer->SetOuter(pPersistentLevel);
	pPlayer->SetActorLocation(pPersistentLevel->GetStartPosition());
}
void World::SetHUD(DirectX11& dx)
{
	if (pHUD == nullptr)
	{
		pHUD = std::make_shared<HUD>(
			shared_from_this(),
			dx,
			GetPlayerController()->GetMouse(),
			(int)AppSettings::windowSize.x,
			(int)AppSettings::windowSize.y
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

	if (pHUD != nullptr)
	{
		if (pHUD->GetTickEnabled())
		{
			pHUD->Tick(dx, deltaSec);
		}
	}

	if (pSubLevel != nullptr)
	{
		pSubLevel->Tick(dx, deltaSec);
	}
	else
	{
		if (pPersistentLevel != nullptr)
		{
			if (pPersistentLevel->GetTickEnabled())
			{
				pPersistentLevel->Tick(dx, deltaSec);
			}
		}
	}

	if (pPlayer != nullptr)
	{
		if (pPlayer->GetTickEnabled())
		{
			pPlayer->Tick(dx, deltaSec);
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
void World::OpenSubLevel(DirectX11& dx, const ELevelId& id)
{
	if (pSubLevel != nullptr)
	{
		auto pLevelFactory = std::make_shared<LevelFactory>();
		pSubLevel = pLevelFactory->Create(dx, id);
	}
}
void World::CloseSubLevel()
{
	pSubLevel.reset();
	pSubLevel = nullptr;
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
std::shared_ptr<Player>& World::GetPlayer()
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
std::shared_ptr<HUD>& World::GetHUD()
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