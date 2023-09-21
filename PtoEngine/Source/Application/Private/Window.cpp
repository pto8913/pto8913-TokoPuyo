
#include "Window.h"
#include "EngineSettings.h"

#include <sstream>
#include "pch.h"

#include "Input/MouseInterface.h"

#include "Core/DirectX.h"
#include "Render/ScreenText.h"

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
    return hInstance;
}

Window::Window(const float& inWidth, const float& inHeight)
    : mClass()
{
    int appWindowX = (int)inWidth;
    int appWindowY = (int)inHeight;

    RECT wr;
    wr.left = 0;
    wr.right = appWindowX + wr.left;
    wr.top = 0;
    wr.bottom = appWindowY + wr.top;
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU, FALSE) == 0)
    {
        return;
    }

    width = wr.right - wr.left;
    height = wr.bottom - wr.top;
    EngineSettings::SetWindowAspectRatio({ float(width) / float(appWindowX), float(height) / float(appWindowY) });

    //width = inWidth;
    //height = inHeight;
    defaultWidth = width;
    defaultHeight = height;
    // create window & get hWnd
    m_hWnd = CreateWindow(
        mClass.GetName(),
        EngineSettings::GetWindowTitle().c_str(),
        WS_CAPTION | WS_SYSMENU | WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,
        nullptr,
        mClass.GetInstance(),
        this
    );
    // check for error
    if (m_hWnd == nullptr)
    {
        return;
    }

}
Window::~Window()
{
    pMouse = nullptr;
    DestroyWindow(m_hWnd);
}

void Window::Init(DirectX11& dx)
{
    pSTOO = ScreenTextOnlyOutput::Make(dx, width, height);
    pRt2D = pSTOO->GetRT2D();

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hWnd), &pRtHWnd);
    pRtHWnd->CreateSolidColorBrush(D2D1::ColorF(1, 0, 0), &pBrushFill);
    pRtHWnd->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &pBrushOutline);

    IDWriteFactory* dwriteFactory = nullptr;

    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&dwriteFactory)
    );

    IDWriteTextFormat* format = nullptr;

    dwriteFactory->CreateTextFormat(
        L"Times New Roman",
        nullptr,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        50,
        L"",
        &format
    );

    std::wstring str = L"pto8913";
    // we want a straight line w/o limit
    dwriteFactory->CreateTextLayout(str.c_str(), str.length(), format, FLT_MAX, 0, &pTextLayout);

    pTextRenderer_Outline = new TextRenderer_Outline(
        pD2DFactory,
        pRtHWnd,
        pBrushOutline,
        pBrushFill
    );
    // newly created windows start off as hidden
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hWnd);
}
void Window::SetTitle(const wchar_t* title)
{
    if (SetWindowText(m_hWnd, title) == 0)
    {
        MessageBox(NULL, L"InValid Window Title", L"hWnd", MB_OK);
        assert(false);
    }
}

HINSTANCE Window::GetHInstance()
{
    return mClass.GetInstance();
}
HWND& Window::GetHWnd()
{
    return m_hWnd;
}
UINT Window::GetWidth() const noexcept
{
    return width;
}
UINT Window::GetHeight() const noexcept
{
    return height;
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
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hWnd, &ps);
        OutputDebugStringA("WM_PAINT\n");
        Render();
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_SIZE:
        pRtHWnd->Resize(D2D1::SizeU(LOWORD(lParam), HIWORD(lParam)));
        return 0;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::Render()
{
    pRtHWnd->BeginDraw();

    pRtHWnd->Clear(D2D1::ColorF(1, 1, 1, 0.f));

    //draw geometry

    //draw phrase using our special render
    pTextLayout->Draw(nullptr, pTextRenderer_Outline, 10, 200);
    //pRt2D->DrawTextLayout(D2D1::Point2F(10, 200), pTextLayout, pBrushFill);
    pRtHWnd->EndDraw();
}
