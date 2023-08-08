#pragma once

#include "DirectX/DirectXHead.h"
#include "Input/ControllerInterface.h"

#include "Slate/SlateInterface.h"

class DirectX11;

class World2D;
class MazeGenerator;

class GameMain
{
public:
	GameMain(DirectX11& dx, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController);
	virtual ~GameMain();

	void DoFrame(DirectX11& dx, float deltaTime);

private:
	// --------------------------
	// Main : Input
	// --------------------------
	void InputUpdate(DirectX11& dx);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<DX::IControllerInterface> m_pController = nullptr;
	DX::IMouseInterface* m_pMouseInterface = nullptr;

	
	std::shared_ptr<MazeGenerator> pMazeGenerator;
	std::shared_ptr<World2D> pActiveMap;
};