#pragma once

#pragma comment(lib, "dwrite.lib")

#include "DirectX/DirectXHead.h"
#include "Window.h"

#include "Input/Camera.h"
#include "Render/Factory/ViewPort.h"

class DirectX11;
class GameMain;

class App
{
public:
	// ------------------------------------------------------------------------------------------------------------
	// Main
	// ------------------------------------------------------------------------------------------------------------

	App();
	~App();

	virtual int Run();

	virtual void CleanUp();

	// Messages
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();

	virtual void OnDeviceLost();

	// ------------------------------------------------------
	// Main : Window
	// ------------------------------------------------------
	virtual void OnWindowSizeChanged(int inWidth, int inHeight);

protected:
	// ------------------------------------------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------------------------------------------
	Window m_Window;

	// Device resources.
	UINT width;
	UINT height;

	GameMain* m_pGameMain;

	std::unique_ptr<Camera> m_pCamera;
	std::unique_ptr<ViewPort> m_pViewPort;
};