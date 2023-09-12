#pragma once

#include "DirectX/DirectXHead.h"

#include "Object/Actor.h"

#include "Engine/Timer.h"
#include "Engine/Delegate.h"

#include "Level/Level.h"

class DirectX11;

class Level;
class GameModeBase;
class GameStateBase;
class PlayerController;
class Actor;
class UserWidget;

class BoxCollision;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameModeChanged, GameModeBase*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerControllerChanged, PlayerController*);

// ------------------------------------------------------------------------------------------------------------
// World
// ------------------------------------------------------------------------------------------------------------
class World : public Object, public std::enable_shared_from_this<World>
{
public:
	World();
	virtual ~World();

	// -----------------------------------
	// Initialize
	// -----------------------------------
	void Init(DirectX11& dx);
protected:
	virtual void SetLevel(DirectX11& dx);
	virtual void SetGameMode(DirectX11& dx);
	virtual void SetGameState(DirectX11& dx);
	virtual void SetPlayerController(DirectX11& dx);
	virtual void SetPlayer(DirectX11& dx);
	virtual void SetHUD(DirectX11& dx);
public:

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	// -----------------------------------
	// Main : Util
	// -----------------------------------
	virtual World* GetWorld() override final;

	TimerManager& GetTimerManager();

	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Actor, TClass>>>
	std::shared_ptr<TClass> SpawnActor(Args&&... args)
	{
		std::shared_ptr<TClass> out = std::make_shared<TClass>(std::forward<Args>(args)...);
		out->SetOuter(pPersistentLevel.get());
		out->SetID(mActorTotalCount);
		++mActorTotalCount;
		AddToObjectCollection(out);

		std::shared_ptr<BoxCollision> collision = out->GetComponent<BoxCollision>();
		if (collision != nullptr)
		{
			pPersistentLevel->GetCollisionCollection().Add(collision);
		}
		return out;
	}
	void AddToObjectCollection(std::shared_ptr<Object> in)
	{
		pPersistentLevel->GetObjectCollection()->Add(in);
	}

	// -----------------------------------
	// Main : Util : Level
	// -----------------------------------
	std::shared_ptr<Level>& GetLevel();
	void ActivateLevel();
	void DeactivateLevel();
	void OpenSubLevel(DirectX11& dx, std::shared_ptr<Level> inNewLevel);
	void CloseSubLevel(DirectX11& dx);

	// -----------------------------------
	// Main : Util : GameMode
	// -----------------------------------
	std::shared_ptr<GameModeBase>& GetGameMode();
	void SetGameMode(DirectX11& dx, std::shared_ptr<GameModeBase>&& NewGameMode);

	// -----------------------------------
	// Main : Util : GameState
	// -----------------------------------
	std::shared_ptr<GameStateBase>& GetGameState();
	std::shared_ptr<PlayerController>& GetPlayerController();

	// -----------------------------------
	// Main : Util : Player
	// -----------------------------------
	std::shared_ptr<Actor>& GetPlayer();
	void ActivatePlayer();
	void DeactivatePlayer();

	// -----------------------------------
	// Main : Util : HUD
	// -----------------------------------
	std::shared_ptr<UserWidget>& GetHUD();
	void ActivateHUD();
	void DeactivateHUD();

	float GetWorldDeltaSec() const noexcept;

	FOnGameModeChanged OnGameModeChanged;
	FOnPlayerControllerChanged OnPlayerControllerChanged;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	TimerManager mTimerManager;

	std::shared_ptr<Level> pPersistentLevel = nullptr;
	/* cache only persistent */
	std::shared_ptr<Level> pCachedPersistentLevel = nullptr;
	std::shared_ptr<GameModeBase> pGameMode = nullptr;
	std::shared_ptr<GameStateBase> pGameState = nullptr;
	std::shared_ptr<PlayerController> pPlayerController = nullptr;
	std::shared_ptr<Actor> pPlayer = nullptr;
	std::shared_ptr<UserWidget> pHUD = nullptr;

	float mWorldDelta = 0.f;

	int mActorTotalCount = 0;
};