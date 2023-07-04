
#include "App.h"

#include "Core/DirectX.h"

#include "Render/World/SkySphere.h"
#include "Object/DrawPlane.h"
#include "Object/DrawSphere.h"
#include "Object/SkyLight.h"

#include "Render/Viewport/ScreenText.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

const std::wstring SHADERPATH = L"Shader/Shader.hlsl";

using namespace DirectX;

App::App(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
	: m_Window(1280, 720, L"pto8913", hInstance, hPrevInstance, lpCmdLine, nCmdShow)
{
	//m_Window.__Tick.Bind<&App::Tick>(*this);
	m_Window.__OnWindowSizeChanged.Bind<&App::OnWindowSizeChanged>(*this);
	m_Window.__OnSuspending.Bind<&App::OnSuspending>(*this);
	m_Window.__OnResuming.Bind<&App::OnResuming>(*this);
	m_Window.__OnActivated.Bind<&App::OnActivated>(*this);
	m_Window.__OnDeactivated.Bind<&App::OnDeactivated>(*this);

	m_pDrawPlane = new DrawPlane(m_Window.GetDX(), 100.f);
	m_pDrawPlane->SetLocation({ 0.f, -50.f, 0.f });

	m_pDrawSphere = new DrawSphere(m_Window.GetDX(), 25.f);
	m_pDrawSphere->SetLocation({ 0.f, 50.f, 0.f });

	m_pScreenText = new ScreenText(m_Window.GetDX(), m_outputWidth, m_outputHeight);
	m_pScreenText->UpdateText(L"Hello World");

	m_pSkyLight = new SkyLight(m_Window.GetDX());

	/* Shader */
	{
		vertexShader = VertexShader::Make(m_Window.GetDX(), SHADERPATH, "VS");
		pixelShader = PixelShader::Make(m_Window.GetDX(), SHADERPATH, "PS");
		vertexShader->Bind(m_Window.GetDX());
		pixelShader->Bind(m_Window.GetDX());
	}
}

int App::Tick()
{
	m_timer.Tick([&]()
		{
		}
	);
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}
	float elapsedTime = float(m_timer.GetElapsedSeconds());

	m_Window.GetDX().BeginFrame(elapsedTime);

	// TODO: Add your game logic here.
	elapsedTime;

	//m_pSkySphere->Update(m_pCamera);


	m_pDrawPlane->ExecuteTasks(m_Window.GetDX());
	m_pDrawSphere->ExecuteTasks(m_Window.GetDX());
	m_pScreenText->ExecuteTasks(m_Window.GetDX());
	m_pSkyLight->ExecuteTasks(m_Window.GetDX());

	HRESULT result = m_Window.GetDX().EndFrame();
	// If the device was reset we must completely reinitialize the renderer.
	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		if (FAILED(result))
		{
			assert(false);
		}
	}
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
	m_timer.ResetElapsedTime();

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
void App::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}
void App::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);
}