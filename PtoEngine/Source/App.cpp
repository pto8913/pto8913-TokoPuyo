
#include "pch.h"

#include "App.h"

#include "Core/DirectX.h"

#include "Timer/Timer.h"

#include "GameMain.h"

#include "Input/Controller.h"

#include "Config.h"

App::App()
	: m_Window((int)Config::windowSize.x, (int)Config::windowSize.y, Config::windowTitle)
{
	DirectX11& dx = m_Window.GetDX();
	dx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pController = std::make_shared<Controller>(dx, m_Window.GetHInstance(), m_Window.GetHWnd());
	m_Window.pController = m_pController;
	m_Window.pMouse = m_pController->GetMouseInterface();

	m_pGameMain = std::make_unique<GameMain>(dx, (UINT)Config::windowSize.x, (UINT)Config::windowSize.y, m_pController);

	m_pWorldTimer = std::make_unique<WorldTimer>();

	/* Viewport */
	{
		UINT x, y;
		m_Window.GetWindowSize(x, y);
		m_pViewPort = std::make_unique<ViewPort>((float)x, (float)y);
		m_pViewPort->Bind(dx);
	}
}
App::~App()
{
	m_pViewPort.reset();
	m_pGameMain.reset();
	m_pWorldTimer.reset();
}

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
int App::Run()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			CoUninitialize();

			return *ecode;
		}

		const auto deltaTime = m_pWorldTimer->GetDelta() * worldTimerSpeed;

		InputUpdate(deltaTime);
		m_pGameMain->DoFrame(m_Window.GetDX(), deltaTime);
	}
}
void App::InputUpdate(float)
{
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	return App().Run();
}