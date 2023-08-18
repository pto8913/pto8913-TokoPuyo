#pragma once

#include <memory>
#include "UI/HUDInterface.h"
#include "Input/ControllerInterface.h"
#include "Engine/Delegate.h"

class DirectX11;

class World2D;
class HUD;

class GameModeBase;
class GameStateBase;

class PlayerController;
class Player;


class GameInstance
{
	friend class App;
public:
	GameInstance() = default;
	virtual ~GameInstance();

	GameInstance(const GameInstance&) = delete;
	GameInstance& operator=(const GameInstance&) = delete;

private:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	/* Initialied from App only once. */
	virtual void Initialize(DirectX11& dx);
	
public:
	// -----------------------------------
	// Main : Utils
	// -----------------------------------
	static GameInstance& Get();
};