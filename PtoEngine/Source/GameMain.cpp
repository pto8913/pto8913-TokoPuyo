
#include "GameMain.h"
#include "GameMode.h"

#include "Window.h"

#include "Object/DrawPlane.h"
#include "Object/DrawSphere.h"
#include "Object/SkySphere.h"
#include "Object/SkyLight.h"

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
Keyboard::InputAction InputF8(DIK_F8);

Keyboard::InputAction InputEsc(DIK_ESCAPE);

#define _DEBUG 0

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

	//m_pDrawPlane = std::make_shared<DrawPlane>(dx, 500.f);
	//m_pDrawPlane->SetLocation({ 0, -50, 0 });

	//m_pDrawSphere = std::make_shared<DrawSphere>(dx, 50.f);
	//m_pDrawSphere->SetLocation({ 0, 50, 0 });

	//m_pSkyLight = std::make_shared<SkyLight>(dx);

	m_pSkySphere = std::make_shared<SkySphere>(dx, 50.f);
}
GameMain::~GameMain()
{
}

void GameMain::DoFrame(DirectX11& dx, float deltaTime)
{
	dx.BeginFrame();

	InputUpdate(dx);

	//m_pSkyLight->ExecuteTasks(dx);

	//m_pDrawSphere->ExecuteTasks(dx);
	//m_pDrawPlane->ExecuteTasks(dx);

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
#include <format>
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
		m_pCameraInterface->AddMoveBackForward(1.f);
	}
	if (InputS)
	{
#if _DEBUG
		OutputDebugStringA("S\n");
#endif
		m_pCameraInterface->AddMoveBackForward(-1.f);
	}
	if (InputA)
	{
#if _DEBUG
		OutputDebugStringA("A\n");
#endif
		m_pCameraInterface->AddMoveLeftRight(-1.f);
	}
	if (InputD)
	{
#if _DEBUG
		OutputDebugStringA("D\n");
#endif
		m_pCameraInterface->AddMoveLeftRight(1.f);
	}
	if (InputQ)
	{
#if _DEBUG
		OutputDebugStringA("Q\n");
#endif
		m_pCameraInterface->AddMoveUpDownward(-1.f);
	}
	if (InputE)
	{
#if _DEBUG
		OutputDebugStringA("E\n");
#endif
		m_pCameraInterface->AddMoveUpDownward(1.f);
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
		OutputDebugStringA("UIONly\n");
#endif
		break;
	case DX::FInputMode::GameOnly:
#if _DEBUG
		OutputDebugStringA("GameONly\n");
#endif
	case DX::FInputMode::GameAndUI:
#if _DEBUG
		OutputDebugStringA("GameAndUI\n");
#endif
		m_pCameraInterface->AddPitch(m_pController->GetMouseDeltaY());
		m_pCameraInterface->AddYaw(m_pController->GetMouseDeltaX());
#if _DEBUG
		OutputDebugStringA(std::format("{} {}\n", m_pController->GetMouseDeltaX(), m_pController->GetMouseDeltaY()).c_str());
#endif
		break;
	default:
		break;
	}
	if (m_pController)
	{
		m_pController->ExecuteTasks(dx);
	}
}