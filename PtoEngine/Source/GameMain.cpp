
#include "GameMain.h"
#include "GameMode.h"

#include "Window.h"

#include "Object/DrawPlane.h"
#include "Object/SkySphere.h"

#include "Input/Keyboard.h"

#include "Math/Math.h"

using namespace DirectX;
using namespace Math;

Keyboard::InputAxis InputW(DIK_W);
Keyboard::InputAxis InputA(DIK_A);
Keyboard::InputAxis InputS(DIK_S);
Keyboard::InputAxis InputD(DIK_D);
Keyboard::InputAxis InputQ(DIK_Q);
Keyboard::InputAxis InputE(DIK_E);
Keyboard::InputAxis InputF8(DIK_F8);

Keyboard::InputAction InputEsc(DIK_ESCAPE);

GameMain::GameMain(DirectX11& dx, UINT windowSizeW, UINT windowSizeH, std::shared_ptr<DX::IControllerInterface> inController)
	: m_pController(inController)
{
#if _DEBUG
	OutputDebugStringA("DEBUG\n");
#else
	OutputDebugStringA("NotDebug\n");
#endif

	m_pMouseInterface = m_pController->GetMouseInterface();
	m_pCameraInterface = m_pController->GetCameraInterface();
	m_pMouseInterface->GetMouseMove().Bind<&GameMain::MouseMove>(*this);

	m_pPlane_Ground = std::make_shared<DrawPlane>(dx, 100.f);
	m_pSkySphere = std::make_shared<SkySphere>(dx, 100.f);
}
GameMain::~GameMain()
{
}

void GameMain::DoFrame(DirectX11& dx, float deltaTime)
{
	dx.BeginFrame();

	InputUpdate(dx);

	m_pPlane_Ground->ExecuteTasks(dx);
	m_pSkySphere->ExecuteTasks(dx);

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

// ------------------------------------------------------------
// Main : Input
// ------------------------------------------------------------
void GameMain::InputUpdate(DirectX11& dx)
{
	if (m_pController)
	{
		m_pController->ExecuteTasks(dx);
	}

	if (InputEsc)
	{
		PostQuitMessage(0);
	}

	if (InputW)
	{
		m_pCameraInterface->AddMoveBackForward(1);
	}
	if (InputS)
	{
		m_pCameraInterface->AddMoveBackForward(-1);
	}
	if (InputA)
	{
		m_pCameraInterface->AddMoveLeftRight(-11);
	}
	if (InputD)
	{
		m_pCameraInterface->AddMoveLeftRight(1);
	}
	if (InputQ)
	{
		m_pCameraInterface->AddMoveUpDownward(1);
	}
	if (InputE)
	{
		m_pCameraInterface->AddMoveUpDownward(-1);
	}
	if (InputF8)
	{
		switch (m_pController->GetInputMode())
		{
		case DX::FInputMode::GameOnly:
			m_pController->SetInputMode(DX::FInputMode::UIOnly);
			break;
		case DX::FInputMode::UIOnly:
			m_pController->SetInputMode(DX::FInputMode::GameOnly);
			break;
		default:
			break;
		}

	}
}
#include <format>
void GameMain::MouseMove(DX::MouseEvent inMouseEvent)
{
	switch (m_pController->GetInputMode())
	{
	case DX::FInputMode::UIOnly:
		break;
	case DX::FInputMode::GameOnly:
	case DX::FInputMode::GameAndUI:
		m_pCameraInterface->AddPitch(m_pController->GetMouseDeltaY());
		m_pCameraInterface->AddYaw(m_pController->GetMouseDeltaX());
		OutputDebugStringA(std::format("{} {}", m_pController->GetMouseDeltaX(), m_pController->GetMouseDeltaY()).c_str());
#if _DEBUG
		OutputDebugStringA(std::format("{} {}", m_pController->GetMouseDeltaX(), m_pController->GetMouseDeltaY()).c_str());
#endif
		break;
	default:
		break;
	}
}