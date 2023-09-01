#pragma once

#include "DirectX/DirectXHead.h"

#include "Engine/Delegate.h"

#include <optional>
#include <memory>

#include "Input/MouseInterface.h"

DECLARE_MULTICAST_DELEGATE(FOnSuspending);
DECLARE_MULTICAST_DELEGATE(FOnResuming);
DECLARE_MULTICAST_DELEGATE(FOnActivated);
DECLARE_MULTICAST_DELEGATE(FOnDeactivated);
DECLARE_MULTICAST_DELEGATE(FOnTick);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWindowSizeChanged, int, int);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnProcessMessage, UINT, WPARAM, LPARAM);

class Window
{
private:
	class WindowClass
	{
	public:
		WindowClass();
		~WindowClass();

		const wchar_t* GetName() noexcept;
		HINSTANCE GetInstance() noexcept;
	private:
		HINSTANCE hInstance = nullptr;
		const wchar_t* wndClassName = L"pto8913 Engine Window";
	};
public:
	Window(const float& inWidth, const float& inHeight);
	~Window();
	
	void SetTitle(const wchar_t* title);

	HINSTANCE GetHInstance();
	HWND& GetHWnd();
	UINT GetWidth() const noexcept;
	UINT GetHeight() const noexcept;

	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;

	bool IsEnableMouse() const noexcept;
	void SetMouseEnabled(bool in);

	static std::optional<int> ProcessMessages() noexcept;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	FOnTick Tick;
	FOnWindowSizeChanged OnWindowSizeChanged;
	FOnSuspending OnSuspending;
	FOnResuming OnResuming;
	FOnActivated OnActivated;
	FOnDeactivated OnDeactivated;
	FOnProcessMessage OnProcessMessage;
private:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	HWND m_hWnd = nullptr;
	WindowClass mClass;

	UINT width = 800;
	UINT height = 600;

	UINT defaultWidth = 800;
	UINT defaultHeight = 600;

public:
	// ----------------------------------
	// State : Controller
	// ----------------------------------
	DX::IMouseInterface* pMouse = nullptr;
private:
	std::vector<BYTE> rawBuffer;
};