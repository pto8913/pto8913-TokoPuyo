#pragma once

#include <memory>
#include "Engine/Delegate.h"
#include "Engine/Timer.h"

class DirectX11;

class World;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnOpenWorld, World*);

class GameInstance
{
	friend class App;
public:
	GameInstance() = default;
	virtual ~GameInstance();

	GameInstance(const GameInstance&) = delete;
	GameInstance& operator=(const GameInstance&) = delete;

protected:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	/* Initialied from App only once. */
	virtual void Initialize(DirectX11& dx);
	virtual void Tick(DirectX11& dx, float deltaSec);

public:
	// -----------------------------------
	// Main : Utils
	// -----------------------------------
	static GameInstance& Get();
	TimerManager& GetTimerManager();

protected:
	// -----------------------------------
	// Main : Utils : World
	// -----------------------------------
	World* GetWorld();
public:
	void OpenWorld(int id);
	FOnOpenWorld OnOpenWorld;
protected:
	virtual void OpenWorldDelay();
protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::unique_ptr<World> pWorld = nullptr;
	int mQueuedOpenWorldID = 0;

	TimerManager mTimerManager;
};