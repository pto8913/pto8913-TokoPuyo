#pragma once

#include "DirectX/DirectXHead.h"

#include "Input/ControllerInterface.h"

#include "Slate/SlateInterface.h"

#include "Config.h"

class DirectX11;
class DrawPlane;
class DrawSphere;
class SkySphere;
class SkyLight;

class GameMain
{
public:
	GameMain(DirectX11& dx, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController);
	~GameMain();

	void DoFrame(DirectX11& dx, float deltaTime);
private:
	
	// ------------------------------------------------------------
	// Main : Input
	// ------------------------------------------------------------
	void InputUpdate(DirectX11& dx);
private:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<DX::IControllerInterface> m_pController = nullptr;
	DX::IMouseInterface* m_pMouseInterface = nullptr;
	DX::ICameraInterface* m_pCameraInterface = nullptr;

	std::shared_ptr<DrawPlane> m_pDrawPlane;
	std::shared_ptr<DrawSphere> m_pDrawSphere;
	std::shared_ptr<SkySphere> m_pSkySphere;
	std::shared_ptr<SkyLight> m_pSkyLight;
};
