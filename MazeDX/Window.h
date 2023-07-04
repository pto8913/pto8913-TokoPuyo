#pragma once

#include "Core/DirectX.h"

#include "Delegate/Delegate.h"

#include <optional>
#include <memory>

DECLARE_MULTICAST_DELEGATE(OnSuspending);
DECLARE_MULTICAST_DELEGATE(OnResuming);
DECLARE_MULTICAST_DELEGATE(OnActivated);
DECLARE_MULTICAST_DELEGATE(OnDeactivated);
DECLARE_MULTICAST_DELEGATE(OnTick);
DECLARE_MULTICAST_DELEGATE_TwoParams(OnWindowSizeChanged, int, int);

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
	Window(int inWidth, int inHeight, const wchar_t* inName, _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);
	~Window();
	
	void SetTitle(const wchar_t* title);
	static std::optional<int> ProcessMessages() noexcept;

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static OnTick __Tick;
	static OnWindowSizeChanged __OnWindowSizeChanged;
	static OnSuspending __OnSuspending;
	static OnResuming __OnResuming;
	static OnActivated __OnActivated;
	static OnDeactivated __OnDeactivated;

	DirectX11& GetDX();

private:
	HWND m_hWnd;
	std::unique_ptr<DirectX11> pDX;

	static const int defaultWidth = 800;
	static const int defaultHeight = 600;
};