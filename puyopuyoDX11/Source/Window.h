#pragma once

#include "Core/DirectX.h"
#include "Object/Mouse.h"

#include "Delegate/Delegate.h"

#include <optional>
#include <memory>

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

		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		HINSTANCE hInstance;
		static WindowClass instance;
		static constexpr const wchar_t* wndClassName = L"pto8913 Engine Window";
	};
public:
	Window(int inWidth, int inHeight, const std::wstring inName);
	~Window();
	
	void SetTitle(const wchar_t* title);
	static std::optional<int> ProcessMessages() noexcept;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static FOnTick Tick;
	static FOnWindowSizeChanged OnWindowSizeChanged;
	static FOnSuspending OnSuspending;
	static FOnResuming OnResuming;
	static FOnActivated OnActivated;
	static FOnDeactivated OnDeactivated;
	static FOnProcessMessage OnProcessMessage;

	void GetWindowSize(UINT& w, UINT& h);

	DirectX11& GetDX();
	HINSTANCE GetHInstance();
	HWND& GetHWnd();

private:
	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
	void ShowMouseCursor();
	void HideMouseCursor();

private:
	HWND m_hWnd;
	std::unique_ptr<DirectX11> pDX;

	Mouse* pMouse;

	UINT width = 800;
	UINT height = 600;

	static UINT defaultWidth;
	static UINT defaultHeight;

	bool bShowMouseCursor = true;
};