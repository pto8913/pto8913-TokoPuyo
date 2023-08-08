#pragma once

#include "Core/DirectX.h"

#include "Delegate/Delegate.h"

#include <optional>
#include <memory>

#include "Input/ControllerInterface.h"

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

	FOnTick Tick;
	FOnWindowSizeChanged OnWindowSizeChanged;
	FOnSuspending OnSuspending;
	FOnResuming OnResuming;
	FOnActivated OnActivated;
	FOnDeactivated OnDeactivated;
	FOnProcessMessage OnProcessMessage;

	void GetWindowSize(UINT& w, UINT& h);

	DirectX11& GetDX();
	HINSTANCE GetHInstance();
	HWND& GetHWnd();

	void ConfineCursor() noexcept;
	void FreeCursor() noexcept;
private:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	HWND m_hWnd;
	std::unique_ptr<DirectX11> pDX;

	UINT width = 800;
	UINT height = 600;

	UINT defaultWidth;
	UINT defaultHeight;

public:
	// ----------------------------------
	// State : Controller
	// ----------------------------------
	std::shared_ptr<DX::IControllerInterface> pController = nullptr;
	DX::IMouseInterface* pMouse = nullptr;
private:
	std::vector<BYTE> rawBuffer;
};