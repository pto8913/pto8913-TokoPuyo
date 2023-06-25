#include "Input/DirectXInput.h"
#include "Input/Camera.h"

DirectXInput::DirectXInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput,
		NULL
	);

	result = m_directInput->CreateDevice(
		GUID_SysKeyboard,
		&m_directKeyBoard,
		NULL
	);

	result = m_directInput->CreateDevice(
		GUID_SysMouse,
		&m_directMouse,
		NULL
	);

	result = m_directKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	result = m_directKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	result = m_directMouse->SetDataFormat(&c_dfDIMouse);
	result = m_directMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
}
DirectXInput::~DirectXInput()
{
	m_directKeyBoard->Unacquire();
	m_directMouse->Unacquire();
	Util::SafeRelease(m_directInput);
}

void DirectXInput::Update(HWND hWnd, double time, Camera* pCamera)
{
	DIMOUSESTATE mouseCurrState;
	BYTE keyBoardState[256];

	m_directKeyBoard->Acquire();
	m_directMouse->Acquire();

	m_directMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	m_directKeyBoard->GetDeviceState(sizeof(keyBoardState), (LPVOID)&keyBoardState);

	if (keyBoardState[DIK_ESCAPE] & 0x80)
		PostMessage(hWnd, WM_DESTROY, 0, 0);

	float speed = 100.0f * (float)time;

	if (keyBoardState[DIK_A] & 0x80)
	{
		pCamera->AddMoveLeftRight(-speed);
	}
	if (keyBoardState[DIK_D] & 0x80)
	{
		pCamera->AddMoveLeftRight(speed);
	}
	if (keyBoardState[DIK_W] & 0x80)
	{
		pCamera->AddMoveBackForward(speed);
	}
	if (keyBoardState[DIK_S] & 0x80)
	{
		pCamera->AddMoveBackForward(-speed);
	}
	if ((mouseCurrState.lX != m_mouseLastState.lX) || (mouseCurrState.lY != m_mouseLastState.lY))
	{
		pCamera->AddYaw(mouseCurrState.lX * 0.001f);
		pCamera->AddPitch(mouseCurrState.lY * 0.001f);

		m_mouseLastState = mouseCurrState;
	}
}