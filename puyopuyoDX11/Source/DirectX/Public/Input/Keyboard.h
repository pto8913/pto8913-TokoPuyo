#pragma once

#include "DirectX/DirectXHead.h"
#include <format>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

class Keyboard
{
public:
	Keyboard() = default;
	Keyboard(HINSTANCE hInstance, HWND hWnd)
	{
		HRESULT result = DirectInput8Create(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_directInput,
			NULL
		);

		result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_directKeyBoard, NULL);
		//result = m_directInput->CreateDevice(GUID_SysMouse, &m_directMouse, NULL);

		result = m_directKeyBoard->SetDataFormat(&c_dfDIKeyboard);
		result = m_directKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

		//result = m_directMouse->SetDataFormat(&c_dfDIMouse);
		//result = m_directMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	}

	~Keyboard()
	{
		m_directKeyBoard->Unacquire();
		//m_directMouse->Unacquire();
		//m_directInput->Release();
		//m_directKeyBoard->Release();
	}

	void Update()
	{
		m_directKeyBoard->Acquire();
		//m_directMouse->Acquire();
		
		//m_directMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseCurrState);
		m_directKeyBoard->GetDeviceState(sizeof(m_keyBoardState), (LPVOID)&m_keyBoardState);
	}

	struct InputAction
	{
	public:
		InputAction(BYTE key) : mKey(key), mActive(false) {}

		operator bool() noexcept
		{
			if (m_keyBoardState[mKey] & 0x80)
			{
				if (!mActive)
				{
					//OutputDebugStringA(std::format("mKey {} Pressed\n", mKey).c_str());
					mActive = true;
					return true;
				}
			}
			else
			{
				mActive = false;
			}
			return false;
		}
	private:
		BYTE mKey;
		bool mActive;
	};

	struct InputAxis
	{
	public:
		InputAxis(BYTE key) : mKey(key) {}

		operator bool() noexcept
		{
			if (m_keyBoardState[mKey] & 0x80)
			{
				return true;
			}
			return false;
		}
	private:
		BYTE mKey;
	};

	//struct InputMouse
	//{
	//public:
	//	InputMouse() = default;
	//	double GetXAxis() const noexcept
	//	{
	//		return m_mouseCurrState.lX;
	//	}
	//	double GetYAxis() const noexcept
	//	{
	//		return m_mouseCurrState.lY;
	//	}
	//	operator bool() noexcept
	//	{
	//		if ((m_mouseCurrState.lX != m_mouseLastState.lX) || (m_mouseCurrState.lY != m_mouseLastState.lY))
	//		{
	//			m_mouseLastState = m_mouseCurrState;
	//			return true;
	//		}
	//		return false;
	//	}
	//};
private:
	LPDIRECTINPUT8 m_directInput;
	IDirectInputDevice8* m_directKeyBoard;
	//IDirectInputDevice8* m_directMouse;

	static BYTE m_keyBoardState[256];
	//static DIMOUSESTATE m_mouseCurrState;
	static DIMOUSESTATE m_mouseLastState;
};