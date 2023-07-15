
#include "pch.h"

#include "App.h"

#include "Core/DirectX.h"

#include "GameMain.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/DepthStencilView.h"
#include "Render/Factory/RenderTargetView.h"

#include "Config.h"

using namespace DirectX;

const std::wstring SHADERPATH = L"Shader/Shader.hlsl";

App::App()
	: m_Window((int)Config::windowSize.x, (int)Config::windowSize.y, Config::windowTitle)
{
	DirectX11& dx = m_Window.GetDX();
	
	//m_Window.__Tick.Bind<&App::Tick>(*this);
	m_Window.OnWindowSizeChanged.Bind<&App::OnWindowSizeChanged>(*this);
	m_Window.OnSuspending.Bind<&App::OnSuspending>(*this);
	m_Window.OnResuming.Bind<&App::OnResuming>(*this);
	m_Window.OnActivated.Bind<&App::OnActivated>(*this);
	m_Window.OnDeactivated.Bind<&App::OnDeactivated>(*this);

	m_Window.GetWindowSize(width, height);

	dx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_pCamera = std::make_unique<Camera>(dx, "A", DirectX::XMFLOAT3(0, 0, 5.f), 9.f, PI / 2.f);

	m_pGameMain = new GameMain(dx, m_Window.GetHInstance(), m_Window.GetHWnd());

	/* Viewport */
	{
		m_pViewPort = std::make_unique<ViewPort>((float)width, (float)height);
		m_pViewPort->Bind(dx, dx.GetContext());
	}
}
App::~App()
{
}

int App::Run()
{
	return m_pGameMain->Run(m_Window.GetDX());
}
void App::OnActivated()
{
	// TODO: App is becoming active window.
}

void App::OnDeactivated()
{
	// TODO: App is becoming background window.
}

void App::OnSuspending()
{
	// TODO: App is being power-suspended (or minimized).
}

void App::OnResuming()
{
	// TODO: App is being power-resumed (or returning from minimize).
}

void App::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	CleanUp();

	MessageBox(NULL, L"DeviceLost", L"Create DeviceLost Error", MB_OK);
}

void App::CleanUp()
{
}

// ------------------------------------------------------
// Window
// ------------------------------------------------------
void App::OnWindowSizeChanged(int inWidth, int inHeight)
{
	width = std::max(inWidth, 1);
	height = std::max(inHeight, 1);
}

int WINAPI wWinMain(_In_ HINSTANCE , _In_opt_ HINSTANCE , _In_ LPWSTR , _In_ int )
{
	return App().Run();
}