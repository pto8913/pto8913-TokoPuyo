
#include "Window.h"

#include <sstream>

#include "pch.h"

OnTick Window::__Tick;
OnWindowSizeChanged Window::__OnWindowSizeChanged;
OnSuspending Window::__OnSuspending;
OnResuming Window::__OnResuming;
OnActivated Window::__OnActivated;
OnDeactivated Window::__OnDeactivated;

LPCWSTR appName = L"MazeDX";

Window::WindowClass Window::WindowClass::instance;

Window::WindowClass::WindowClass()
    : hInstance(GetModuleHandle(nullptr))
{
    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr))
    {
        assert(false);
    }

    //// Register class and create window
    //// Register class
    //WNDCLASSEXW wc = {};
    //wc.cbSize        = sizeof(wc);
    //wc.style         = CS_OWNDC;
    //wc.lpfnWndProc   = WndProc;
    //wc.cbClsExtra    = 0;
    //wc.cbWndExtra    = 0;
    //wc.hInstance     = hInstance;
    //wc.hIcon         = LoadIconW(hInstance, L"IDI_ICON");
    //wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    //wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    //wc.lpszClassName = GetName();
    //wc.hIconSm       = LoadIconW(wc.hInstance, L"IDI_ICON");
    //RegisterClassEx(&wc);

    // Register class and create window
    // Register class
    WNDCLASSEXW wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEXW);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIconW(hInstance, L"IDI_ICON");
    wcex.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wcex.lpszClassName = GetName();
    wcex.hIconSm       = LoadIconW(wcex.hInstance, L"IDI_ICON");
    RegisterClassExW(&wcex);
}
Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const wchar_t* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}
HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return instance.hInstance;
}

Window::Window(int inWidth, int inHeight, const wchar_t* inName)
{
    RECT wr;
    wr.left = 100;
    wr.right = inWidth + wr.left;
    wr.top = 100;
    wr.bottom = inHeight + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
    {
        return;
    }

    float width = wr.right - wr.left;
    float height = wr.bottom - wr.top;
    // create window & get hWnd
    m_hWnd = CreateWindow(
        WindowClass::GetName(), 
        inName,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        width, 
        height,
        nullptr, 
        nullptr, 
        WindowClass::GetInstance(), 
        this
    );
    // check for error
    if (m_hWnd == nullptr)
    {
        return;
    }
    // newly created windows start off as hidden
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    pDX = std::make_unique<DirectX11>(WindowClass::GetInstance(), m_hWnd, width, height);

    // register mouse raw input device
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01; // mouse page
    rid.usUsage = 0x02; // mouse usage
    rid.dwFlags = 0;
    rid.hwndTarget = nullptr;
    if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    {
        return;
    }
    
}
Window::~Window()
{
    DestroyWindow(m_hWnd);
}

void Window::SetTitle(const wchar_t* title)
{
    if (SetWindowText(m_hWnd, title) == 0)
    {
        MessageBox(NULL, L"InValid Window Title", L"hWnd", MB_OK);
        assert(false);
    }
}

std::optional<int> Window::ProcessMessages() noexcept
{
    MSG msg;
    // while queue has messages, remove and dispatch them (but do not block on empty queue)
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        // check for quit because peekmessage does not signal this via return val
        if (msg.message == WM_QUIT)
        {
            // return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
            return (int)msg.wParam;
        }

        // TranslateMessage will post auxilliary WM_CHAR messages from key msgs
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // return empty optional when not quitting app
    return {};
}

// Windows procedure
LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    static bool s_fullscreen = false;
    // TODO: Set s_fullscreen to true if defaulting to fullscreen.

    switch (message)
    {
    case WM_PAINT:
        if (s_in_sizemove)
        {
            __Tick.Broadcast();
        }
        else
        {
            PAINTSTRUCT ps;
            std::ignore = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
                if (!s_in_suspend)
                {
                    __OnSuspending.Broadcast();
                }
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend)
            {
                __OnResuming.Broadcast();
            }
            s_in_suspend = false;
        }
        else if (!s_in_sizemove)
        {
            __OnWindowSizeChanged.Broadcast(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        RECT rc;
        GetClientRect(hWnd, &rc);

        __OnWindowSizeChanged.Broadcast(rc.right - rc.left, rc.bottom - rc.top);
        break;

    case WM_GETMINMAXINFO:
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_ACTIVATEAPP:
        if (wParam)
        {
            __OnActivated.Broadcast();
        }
        else
        {
            __OnDeactivated.Broadcast();
        }
        break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend)
            {
                __OnSuspending.Broadcast();
            }
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend)
                {
                    __OnResuming.Broadcast();
                }
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                ShowWindow(hWnd, SW_SHOWNORMAL);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, defaultWidth, defaultHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }
        break;

    case WM_MENUCHAR:
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

DirectX11& Window::GetDX()
{
    if (!pDX)
    {

    }
    return *pDX;
}