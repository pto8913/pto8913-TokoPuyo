#pragma once

#include "DirectX/DirectXHead.h"

#include "Engine/Delegate.h"

#include <optional>
#include <memory>

#include "Input/MouseInterface.h"

#include <d2d1.h>
#include <dwrite.h>
#include "Slate/TextRenderer_Outline.h"

DECLARE_MULTICAST_DELEGATE(FOnSuspending);
DECLARE_MULTICAST_DELEGATE(FOnResuming);
DECLARE_MULTICAST_DELEGATE(FOnActivated);
DECLARE_MULTICAST_DELEGATE(FOnWM_PAINT);
DECLARE_MULTICAST_DELEGATE(FOnDeactivated);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWindowSizeChanged, int, int);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnProcessMessage, UINT, WPARAM, LPARAM);
class DirectX11;
class TextRenderer_Outline;

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
	void Render();
	void Init(DirectX11& dx);
	static std::optional<int> ProcessMessages() noexcept;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	FOnWindowSizeChanged OnWindowSizeChanged;
	FOnSuspending OnSuspending;
	FOnResuming OnResuming;
	FOnActivated OnActivated;
	FOnDeactivated OnDeactivated;
	FOnProcessMessage OnProcessMessage;

	FOnWM_PAINT OnWM_PAINT;
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

	ID2D1Factory* pD2DFactory;
	ID2D1SolidColorBrush* pBrushFill;
	ID2D1SolidColorBrush* pBrushOutline;
	ID2D1HwndRenderTarget* pRtHWnd;
	IDWriteTextLayout* pTextLayout;
	TextRenderer_Outline* pTextRenderer_Outline;

	std::shared_ptr<class ScreenTextOnlyOutput> pSTOO;
	ID2D1RenderTarget* pRt2D;
public:
	// ----------------------------------
	// State : Controller
	// ----------------------------------
	DX::IMouseInterface* pMouse = nullptr;
private:
	std::vector<BYTE> rawBuffer;
};