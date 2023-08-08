
#include "GameMain.h"

#include "DirectX/Public/Core/DirectX.h"

#include "Core/GameMode.h"
#include "Core/Window.h"

#include "Object/DrawPlane.h"
#include "Object/DrawSphere.h"
#include "Object/SkySphere.h"
#include "Object/SkyLight.h"

#include "Input/Keyboard.h"

#include "Algorithm/MazeGenerator.h"

using namespace DirectX;

Keyboard::InputAxis InputW(DIK_W);
Keyboard::InputAxis InputA(DIK_A);
Keyboard::InputAxis InputS(DIK_S);
Keyboard::InputAxis InputD(DIK_D);
Keyboard::InputAxis InputQ(DIK_Q);
Keyboard::InputAxis InputE(DIK_E);
Keyboard::InputAction InputF8(DIK_F8);

Keyboard::InputAction InputEsc(DIK_ESCAPE);

#define _DEBUG 1

#if _DEBUG
#include <format>
#endif

GameMain::GameMain(DirectX11& dx, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController)
	: m_pController(inController)
{
#if _DEBUG
	OutputDebugStringA("DEBUG\n");
#else
	OutputDebugStringA("NotDebug\n");
#endif

	m_pMouseInterface = m_pController->GetMouseInterface();
	m_pController->SetInputMode(DX::FInputMode::UIOnly);

	pMazeGenerator = std::make_shared<MazeGenerator>();
	pMazeGenerator->GenerateMaze();
}
GameMain::~GameMain()
{
}

void GameMain::DoFrame(DirectX11& dx, float deltaTime)
{
	dx.BeginFrame();

	InputUpdate(dx);

	HRESULT result = dx.EndFrame();
	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	{
		//OnDeviceLost();
	}
	else
	{
		if (FAILED(result))
		{
			assert(false);
		}
	}
}

void GameMain::InputUpdate(DirectX11& dx)
{
	if (InputEsc)
	{
		PostQuitMessage(0);
	}

	if (InputW)
	{
#if _DEBUG
		OutputDebugStringA("W\n");
#endif
	}
	if (InputS)
	{
#if _DEBUG
		OutputDebugStringA("S\n");
#endif
	}
	if (InputA)
	{
#if _DEBUG
		OutputDebugStringA("A\n");
#endif
	}
	if (InputD)
	{
#if _DEBUG
		OutputDebugStringA("D\n");
#endif
	}
	if (InputQ)
	{
#if _DEBUG
		OutputDebugStringA("Q\n");
#endif
	}
	if (InputE)
	{
#if _DEBUG
		OutputDebugStringA("E\n");
#endif
	}
	if (InputF8)
	{
#if _DEBUG
		OutputDebugStringA("F8\n");
#endif
		switch (m_pController->GetInputMode())
		{
		case DX::FInputMode::GameOnly:
			m_pController->SetInputMode(DX::FInputMode::UIOnly);
			break;
		default:
			m_pController->SetInputMode(DX::FInputMode::GameOnly);
			break;
		}
	}
	switch (m_pController->GetInputMode())
	{
	case DX::FInputMode::UIOnly:
#if _DEBUG
		//OutputDebugStringA("UIONly\n");
#endif
		break;
	case DX::FInputMode::GameOnly:
#if _DEBUG
		//OutputDebugStringA("GameONly\n");
#endif
	case DX::FInputMode::GameAndUI:
#if _DEBUG
		////OutputDebugStringA("GameAndUI\n");
#endif
#if _DEBUG
		//OutputDebugStringA(std::format("{} {}\n", m_pController->GetMouseDeltaX(), m_pController->GetMouseDeltaY()).c_str());
#endif
		break;
	default:
		break;
	}
	switch (m_pController->GetInputMode())
	{
	case DX::FInputMode::UIOnly:
		break;
	case DX::FInputMode::GameOnly:
	case DX::FInputMode::GameAndUI:
		break;
	default:
		break;
	}
	if (m_pController)
	{
		m_pController->ExecuteTasks(dx);
	}
}