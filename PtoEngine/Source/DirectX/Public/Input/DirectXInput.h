#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "DirectX/DirectXHead.h"

#pragma comment(lib, "dinput8.lib")

#define KEYDOWN(name, key) ( name[key] & 0x80 )
#define BUTTONDOWN(device, key) ( device.rgbButtons[key] & 0x80 )

class Camera;

class DirectXInput
{
	enum E_Mouse {
		E_Mouse_Left,
		E_Mouse_Right,
	};
public:
	DirectXInput(HINSTANCE hInstance, HWND hWnd);
	~DirectXInput();

	void ExecuteTasks(float time, Camera* pCamera);

private:
	IDirectInputDevice8* m_directKeyBoard;
	IDirectInputDevice8* m_directMouse;

	DIMOUSESTATE m_mouseLastState;
	LPDIRECTINPUT8 m_directInput;
};