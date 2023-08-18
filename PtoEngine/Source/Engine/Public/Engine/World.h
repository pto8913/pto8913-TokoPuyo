#pragma once

#include "DirectX/DirectXHead.h"

#include "Object/Object.h"

#include "Engine/Timer.h"
#include "Engine/Delegate.h"

class DirectX11;

class GameModeBase;
class GameStateBase;
class PlayerController;
class Level;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, const std::shared_ptr<GameModeBase>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerChanged, const std::shared_ptr<PlayerController>&);

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
class World : public Object
{
public:
	World(DirectX11& dx);
	virtual ~World();

	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	TimerManager& GetTimerManager();
protected:
	virtual void SetGameMode(DirectX11& dx) = 0;
	virtual void SetGameState(DirectX11& dx) = 0;
	virtual void SetPlayerController(DirectX11& dx) = 0;
	virtual void SetLevel(DirectX11& dx) = 0;

public:
	// -----------------------------------
	// Main : GameMode
	// -----------------------------------
	std::shared_ptr<GameModeBase>& GetGameMode();
	void SetGameMode(DirectX11& dx, std::shared_ptr<GameModeBase>&& NewGameMode);

public:
	// -----------------------------------
	// Main : Util
	// -----------------------------------
	std::shared_ptr<GameModeBase>& GetGameMode();
	std::shared_ptr<GameStateBase>& GetGameState();
	std::shared_ptr<PlayerController>& GetPlayerController();
	std::shared_ptr<Level>& GetLevel();

	FOnGameModeChanged OnGameModeChanged;
	FOnPlayerControllerChanged OnPlayerControllerChanged;
protected:
	TimerManager mTimerManager;

	std::shared_ptr<GameModeBase> pGameMode = nullptr;
	std::shared_ptr<GameStateBase> pGameState = nullptr;
	std::shared_ptr<PlayerController> pPlayerController = nullptr;
	std::shared_ptr<Level> pPersistentLevel = nullptr;
};