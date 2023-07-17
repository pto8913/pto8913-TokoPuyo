#pragma once

#pragma comment(lib, "dwrite.lib")

#include "DirectX/DirectXHead.h"
#include "Window.h"

#include "Render/Factory/ViewPort.h"

class DirectX11;
class GameMain;
class WorldTimer;
class Controller;

class App
{
public:
	App();
	~App();

	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------
	virtual int Run();
	void InputUpdate(float deltaTime);

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	Window m_Window;

	std::unique_ptr<GameMain> m_pGameMain;
	std::unique_ptr<ViewPort> m_pViewPort;

	std::shared_ptr<Controller> m_pController;

	// ------------------------------------------------------
	// State : WorldTimer
	// ------------------------------------------------------
	std::unique_ptr<WorldTimer> m_pWorldTimer;
	float worldTimerSpeed = 1.f;
};