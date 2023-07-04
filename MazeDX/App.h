#pragma once

#include "DirectX/DirectXHead.h"
#include "StepTimer.h"
#include "Window.h"

class DrawPlane;
class DrawSphere;
class ScreenText;
class PixelShader;
class PixelShader;
class VertexShader;

class SkyLight;

class DirectX11;

class App
{
public:
	App();

	virtual int Tick();
	virtual void DoFrame();

	// Messages
	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();

	virtual void OnDeviceLost();

	virtual void CleanUp();

	// ------------------------------------------------------
	// Window
	// ------------------------------------------------------
	void GetDefaultSize(int& width, int& height) const noexcept;
	virtual void OnWindowSizeChanged(int width, int height);

protected:
	Window m_Window;

	// Device resources.
	UINT m_outputWidth;
	UINT m_outputHeight;

	DX::StepTimer m_timer;

	DrawPlane* m_pDrawPlane;
	DrawSphere* m_pDrawSphere;
	ScreenText* m_pScreenText;
	SkyLight* m_pSkyLight;

	// ---------------------------------------
	std::shared_ptr<PixelShader> pixelShader;
	std::shared_ptr<VertexShader> vertexShader;
	// ---------------------------------------
};