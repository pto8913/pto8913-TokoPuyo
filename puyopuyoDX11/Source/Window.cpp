
#include "Window.h"
#include <sstream>
#include "pch.h"

#include "Input/MouseInterface.h"

FOnTick Window::Tick;
FOnWindowSizeChanged Window::OnWindowSizeChanged;
FOnSuspending Window::OnSuspending;
FOnResuming Window::OnResuming;
FOnActivated Window::OnActivated;
FOnDeactivated Window::OnDeactivated;
FOnProcessMessage Window::OnProcessMessage;

Window::WindowClass Window::WindowClass::instance;

Window::WindowClass::WindowClass()
    : hInstance(GetModuleHandle(nullptr))
{
    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
    if (FAILED(hr))
    {
        assert(false);
    }

    WNDCLASSEXW wcex = {};
    wcex.cbSize        = sizeof(WNDCLASSEXW);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = HandleMsgSetup;
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

Window::Window(int inWidth, int inHeight, const std::wstring inName)
{
    RECT wr;
    wr.left = 100;
    wr.right = inWidth + wr.left;
    wr.top = 100;
    wr.bottom = inHeight + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW, FALSE) == 0)
    {
        return;
    }

    width = wr.right - wr.left;
    height = wr.bottom - wr.top;

    //width = inWidth;
    //height = inHeight;
    defaultWidth = width;
    defaultHeight = height;
    // create window & get hWnd
    m_hWnd = CreateWindow(
        WindowClass::GetName(), 
        inName.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPEDWINDOW,
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

    //// register mouse raw input device
    //RAWINPUTDEVICE rid;
    //rid.usUsagePage = 0x01; // mouse page
    //rid.usUsage = 0x02; // mouse usage
    //rid.dwFlags = 0;
    //rid.hwndTarget = nullptr;
    //if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
    //{
    //    return;
    //}
}
Window::~Window()
{
    pController = nullptr;
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

void Window::GetWindowSize(UINT& w, UINT& h)
{
    w = width;
    h = height;
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

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
    if (msg == WM_NCCREATE)
    {
        // extract ptr to window class from creation data
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        // set WinAPI-managed user data to store ptr to window instance
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        // set message proc to normal (non-setup) handler now that setup is finished
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        // forward message to window instance handler
        return pWnd->WndProc(hWnd, msg, wParam, lParam);
    }
    // if we get a message before the WM_NCCREATE message, handle with default handler
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // retrieve ptr to window instance
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    // forward message to window instance handler
    return pWnd->WndProc(hWnd, msg, wParam, lParam);
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
            Tick.Broadcast();
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
                    OnSuspending.Broadcast();
                }
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
            if (s_in_suspend)
            {
                OnResuming.Broadcast();
            }
            s_in_suspend = false;
        }
        else if (!s_in_sizemove)
        {
            OnWindowSizeChanged.Broadcast(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
    {
        s_in_sizemove = false;
        RECT rc;
        GetClientRect(hWnd, &rc);

        OnWindowSizeChanged.Broadcast(rc.right - rc.left, rc.bottom - rc.top);
        break;
    }
    case WM_GETMINMAXINFO:
    {
        if (lParam)
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;
    }
    case WM_ACTIVATEAPP:
    {
        if (wParam)
        {
            OnActivated.Broadcast();

        }
        else
        {
            OnDeactivated.Broadcast();
        }
        break;
    }
    case WM_POWERBROADCAST:
    {
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
            if (!s_in_suspend)
            {
                OnSuspending.Broadcast();
            }
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
                if (s_in_suspend)
                {
                    OnResuming.Broadcast();
                }
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_SYSKEYDOWN:
    {
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
    }
    case WM_MENUCHAR:
    {
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);
    }
    case WM_ACTIVATE:
    {
        if (pController != nullptr)
        {
            if (!pController->IsEnableMouse())
            {
                if (wParam & WA_ACTIVE)
                {
                    ConfineCursor();
                    pController->SetMouseEnabled(false);
                }
                else
                {
                    FreeCursor();
                    pController->SetMouseEnabled(true);
                }
            }
            break;
        }
    }

    // --------------------------------------------------------------------------
    // Mouse Keyboard
    // --------------------------------------------------------------------------

    // --------------------------------------------------------------------------
    // Mouse Messages
    // --------------------------------------------------------------------------
    case WM_MOUSEMOVE:
    {
        if (pController != nullptr)
        {
            const POINTS pt = MAKEPOINTS(lParam);
            if (!pController->IsEnableMouse())
            {
                if (!pMouse->IsInWindow())
                {
                    SetCapture(hWnd);

                    pMouse->OnMouseEnter();
                    pController->SetMouseEnabled(false);
                }
                break;
            }
            if (pt.x >= 0 && pt.y < width && pt.y >= 0 && pt.y < height)
            {
                pMouse->OnMouseMove(pt.x, pt.y);
                if (!pMouse->IsInWindow())
                {
                    SetCapture(hWnd);
                    pMouse->OnMouseEnter();
                }
            }
            else
            {
                if (wParam & (MK_LBUTTON | MK_RBUTTON))
                {
                    pMouse->OnMouseMove(pt.x, pt.y);
                }
                else
                {
                    ReleaseCapture();
                    pMouse->OnMouseLeave();
                }
            }
            break;
        }
    }
    case WM_LBUTTONDOWN:
    {
        SetForegroundWindow(hWnd);
        if (pController != nullptr)
        {
            if (!pController->IsEnableMouse())
            {
                ConfineCursor();
                pController->SetMouseEnabled(false);
            }
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnLeftPressed(pt.x, pt.y);
            break;
        }
    }
    case WM_LBUTTONUP:
    {
        if (pController != nullptr)
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnLeftReleased(pt.x, pt.y);
            if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                pMouse->OnMouseLeave();
            }
            break;
        }
    }
    case WM_RBUTTONDOWN:
    {
        if (pController != nullptr)
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnRightPressed(pt.x, pt.y);
            break;
        }
    }
    case WM_RBUTTONUP:
    {
        if (pController != nullptr)
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnRightReleased(pt.x, pt.y);
            if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
            {
                ReleaseCapture();
                pMouse->OnMouseLeave();
            }
            break;
        }
    }
    case WM_MOUSEWHEEL:
    {
        if (pController != nullptr)
        {
            const POINTS pt = MAKEPOINTS(lParam);
            const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            pMouse->OnWheelDelta(pt.x, pt.y, delta);
            break;
        }
    }
    // ------------------------------------
    // Raw Mouse Messages
    // ------------------------------------
    case WM_INPUT:
    {
        if (pController != nullptr)
        {
            if (!pMouse->IsRawEnabled())
            {
                break;
            }
            UINT size;
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == -1)
            {
                break;
            }
            rawBuffer.resize(size);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) != size)
            {
                break;
            }
            auto&& raw = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
            if (raw.header.dwType == RIM_TYPEMOUSE && (raw.data.mouse.lLastX != 0 || raw.data.mouse.lLastY != 0))
            {
                pMouse->OnRawDelta(raw.data.mouse.lLastX, raw.data.mouse.lLastY);
            }
            break;
        }
    }
    }

    if (pController != nullptr)
    {
        if (pMouse->IsLeftPressed())
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnLeftHeld(pt.x, pt.y);
        }
        if (pMouse->IsRightPressed())
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnRightHeld(pt.x, pt.y);
        }
        if (pMouse->IsWheelPressed())
        {
            const POINTS pt = MAKEPOINTS(lParam);
            pMouse->OnWheelHeld(pt.x, pt.y);
        }
    }
    OnProcessMessage.Broadcast(message, wParam, lParam);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

DirectX11& Window::GetDX()
{
    if (!pDX)
    {

    }
    return *pDX;
}

HINSTANCE Window::GetHInstance()
{
    return WindowClass::GetInstance();
}
HWND& Window::GetHWnd()
{
    return m_hWnd;
}

void Window::ConfineCursor() noexcept
{
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    MapWindowPoints(m_hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
    ClipCursor(&rect);
}
void Window::FreeCursor() noexcept
{
    ClipCursor(nullptr);
}