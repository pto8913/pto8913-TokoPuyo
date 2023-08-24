#pragma once

#include <Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")

class Keyboard
{
public:
	Keyboard() = default;
	Keyboard(HINSTANCE hInstance, HWND hWnd)
	{
		DirectInput8Create(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&m_directInput,
			NULL
		);

		m_directInput->CreateDevice(GUID_SysKeyboard, &m_directKeyBoard, NULL);

		m_directKeyBoard->SetDataFormat(&c_dfDIKeyboard);
		m_directKeyBoard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	}

	~Keyboard()
	{
		m_directKeyBoard->Unacquire();
		//m_directInput->Release();
		//m_directKeyBoard->Release();
	}

	void Update()
	{
		m_directKeyBoard->Acquire();

		m_directKeyBoard->GetDeviceState(sizeof(m_keyBoardState), (LPVOID)&m_keyBoardState);
	}

	LPDIRECTINPUT8& GetDirectInput()
	{
		return m_directInput;
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
#if _DEBUG
					//OutputDebugStringA(std::format("mKey {} Pressed\n", mKey).c_str());
#endif
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
				if (isPressedFlip)
				{
					isPressed = true;
					isPressedFlip = false;
				}
				else
				{
					isPressed = false;
				}
				return true;
			}
			isPressedFlip = true;
			isPressed = false;
			return false;
		}
		bool IsPressed()
		{
			return isPressed;
		}
	private:
		BYTE mKey;
		bool isPressed = false;
		bool isPressedFlip = true;
	};

private:
	LPDIRECTINPUT8 m_directInput;
	IDirectInputDevice8* m_directKeyBoard;

	static BYTE m_keyBoardState[256];
};