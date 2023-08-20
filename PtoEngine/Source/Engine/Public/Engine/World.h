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

class Actor;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, const std::shared_ptr<GameModeBase>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerChanged, const std::shared_ptr<PlayerController>&);

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
class World : public Object, public std::enable_shared_from_this<World>
{
public:
	World();
	virtual ~World();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void Init(DirectX11& dx);

	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	virtual std::shared_ptr<World> GetWorld() override final;

	TimerManager& GetTimerManager();

	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Actor, TClass>>>
	std::shared_ptr<TClass> SpawnActor(Args&&... args)
	{
		std::shared_ptr<TClass> out = std::make_shared<TClass>(std::forward<Args>(args)...);
		out->SetOuter(pPersistentLevel);
		return out;
	}
protected:
	virtual void SetLevel(DirectX11& dx);
	virtual void SetGameMode(DirectX11& dx);
	virtual void SetGameState(DirectX11& dx);
	virtual void SetPlayerController(DirectX11& dx);

public:
	// -----------------------------------
	// Main : Util
	// -----------------------------------
	std::shared_ptr<GameModeBase>& GetGameMode();
	void SetGameMode(DirectX11& dx, std::shared_ptr<GameModeBase>&& NewGameMode);

	std::shared_ptr<GameStateBase>& GetGameState();
	std::shared_ptr<PlayerController>& GetPlayerController();
	std::shared_ptr<Level>& GetLevel();

	float GetWorldDeltaSec() const noexcept;

	FOnGameModeChanged OnGameModeChanged;
	FOnPlayerControllerChanged OnPlayerControllerChanged;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------

	TimerManager mTimerManager;

	std::shared_ptr<GameModeBase> pGameMode = nullptr;
	std::shared_ptr<GameStateBase> pGameState = nullptr;
	std::shared_ptr<PlayerController> pPlayerController = nullptr;
	std::shared_ptr<Level> pPersistentLevel = nullptr;

	float mWorldDelta = 0.f;
};