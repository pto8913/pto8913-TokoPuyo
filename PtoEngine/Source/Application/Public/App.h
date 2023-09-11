/*
* Application execution order.
* 
* 1. open App.
* 2. create GameInstance from App.
* 3. create GameMode from GameInstance.
* 4. create GameState from GameMode.
* 5. create World from GameState.
* 6. create PlayerController from GameMode.
* 7. create Player from PlayerController.
* 8. start player control. 
* 
*/

#pragma once

#pragma comment(lib, "dwrite.lib")


#include "DirectX/DirectXHead.h"

#include "Render/ViewPort.h"

#include "Window.h"

class DirectX11;

class WorldTimer;

class PlayerController;
class World;
class PtoGameInstance;

class App
{
public:
	App();
	~App();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	int Run();
private:
	void InputUpdate(DirectX11& dx);

	// -----------------------------------
	// Main : GameMode
	// -----------------------------------
	void OnPlayerControllerChanged(const std::shared_ptr<PlayerController>& pPlayerController);
	void OnWorldChanged(const std::shared_ptr<World>& NewWorld);

	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	std::unique_ptr<DirectX11> pDX = nullptr;

	std::unique_ptr<ViewPort> pViewPort = nullptr;
	bool bIsInitialized = false;

	// -----------------------------------
	// State : Window
	// -----------------------------------
	Window mWindow;
	PtoGameInstance* pGameInstance = nullptr;

	// -----------------------------------
	// State : AppTimer
	// -----------------------------------
	std::unique_ptr<WorldTimer> pAppTimer = nullptr;
	float appTimerSpeed = 1.f;
};