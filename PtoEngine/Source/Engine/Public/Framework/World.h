#pragma once

#include "Graphics/DirectXHead.h"

#include "Object/Actor.h"

#include "Engine/Timer.h"
#include "Engine/Delegate.h"

#include "Level/Level.h"

#include "UI/WidgetManager.h"

class DirectX11;

class Level;
class GameModeBase;
class GameStateBase;
class PlayerController;
class Actor;
class UserWidget;

class WidgetManager;

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

protected:
	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Object, TClass>>>
	std::shared_ptr<TClass> __NewObject(Args&& ...args)
	{
		std::shared_ptr<TClass> out = std::make_shared<TClass>(std::forward<Args>(args)...);
		out->SetOuter(pPersistentLevel.get());
		out->SetID(mActorTotalCount);
		++mActorTotalCount;
		AddToObjectManager(out);

		out->NativeOnInitialized(*pDX);
		
		return std::move(out);
	}
public:
	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Object, TClass>>>
	TClass* NewObject(Args&& ...args)
	{
		return std::move(__NewObject<TClass>(std::forward<Args>(args)...).get());
	}
protected:
	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Actor, TClass>>>
	std::shared_ptr<TClass> __SpawnActor(Args&&... args)
	{
		auto out = __NewObject<TClass>(std::forward<Args>(args)...);
		return std::move(out);
	}
public:
	template<class TClass, typename ...Args, typename = typename std::enable_if_t<std::is_base_of_v<Actor, TClass>>>
	TClass* SpawnActor(Args&&... args)
	{
		return std::move(__SpawnActor<TClass>(std::forward<Args>(args)...).get());
	}
	template<class TClass, typename ...Args, typename = std::enable_if_t<std::is_base_of_v<UserWidget, TClass>>>
	TClass* CreateWidget(Object* inOwner, Args&& ...args)
	{
		auto out = pWidgetManager->CreateWidget<TClass>(mActorTotalCount, inOwner, std::forward<Args>(args)...);
		++mActorTotalCount;
		return std::move(out);
	};
	void AddToObjectManager(std::shared_ptr<Object> in);

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "World" + "#"s + std::to_string(mID);
	}

	// -----------------------------------
	// Main : Util : Level
	// -----------------------------------
	Level* GetLevel();
	void ActivateLevel();
	void DeactivateLevel();
	void OpenSubLevel(DirectX11& dx, std::shared_ptr<Level> inNewLevel);
	void CloseSubLevel(DirectX11& dx);

	// -----------------------------------
	// Main : Util : GameMode
	// -----------------------------------
	GameModeBase* GetGameMode();
	void SetGameMode(DirectX11& dx, std::shared_ptr<GameModeBase>&& NewGameMode);

	// -----------------------------------
	// Main : Util : GameState
	// -----------------------------------
	GameStateBase* GetGameState();
	PlayerController* GetPlayerController();

	// -----------------------------------
	// Main : Util : Player
	// -----------------------------------
	Actor* GetPlayer();
	void ActivatePlayer();
	void DeactivatePlayer();

	// -----------------------------------
	// Main : Util : HUD
	// -----------------------------------
	UserWidget* GetHUD();
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

	DirectX11* pDX = nullptr;

	std::shared_ptr<Level> pPersistentLevel = nullptr;
	/* cache only persistent */
	std::shared_ptr<Level> pCachedPersistentLevel = nullptr;
	std::shared_ptr<GameModeBase> pGameMode = nullptr;
	std::shared_ptr<GameStateBase> pGameState = nullptr;
	std::shared_ptr<PlayerController> pPlayerController = nullptr;
	std::shared_ptr<Actor> pPlayer = nullptr;
	UserWidget* pHUD = nullptr;
	std::shared_ptr<WidgetManager> pWidgetManager = nullptr;

	float mWorldDelta = 0.f;

	int mActorTotalCount = 0;
};