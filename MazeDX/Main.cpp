// 
// Main.cpp
// 

#include "pch.h"

//#include "Core/DirectX.h"
#include "App.h"

using namespace DirectX;

#ifdef __clang__
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wswitch-enum"
#endif

#pragma warning(disable : 4061)

LPCWSTR g_szAppName = L"MazeDX";

//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ExitGame() noexcept;

//DirectX11* DirectX11Instance;
//App Application;

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return App(hInstance, hPrevInstance, lpCmdLine, nCmdShow).Tick();
}

//// Entry point
//int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    if (!XMVerifyCPUSupport())
//    {
//        return 1;
//    }
//
//    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
//    if (FAILED(hr))
//    {
//        return 1;
//    }
//
//    // Register class and create window
//    // Register class
//    WNDCLASSEXW wcex = {};
//    wcex.cbSize = sizeof(WNDCLASSEXW);
//    wcex.style = CS_HREDRAW | CS_VREDRAW;
//    wcex.lpfnWndProc = WndProc;
//    wcex.hInstance = hInstance;
//    wcex.hIcon = LoadIconW(hInstance, L"IDI_ICON");
//    wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
//    wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
//    wcex.lpszClassName = L"MazeDXWindowClass";
//    wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");
//    if (!RegisterClassExW(&wcex))
//    {
//        return 1;
//    }
//
//    // Create window
//    UINT w = 800, h = 600;
//
//    RECT rc = { 0, 0, static_cast<LONG>(w), static_cast<LONG>(h) };
//
//    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//
//    const UINT width = rc.right - rc.left;
//    const UINT height = rc.bottom - rc.top;
//    HWND hwnd = CreateWindowExW(
//        0, 
//        L"MazeDXWindowClass", 
//        g_szAppName, 
//        WS_OVERLAPPEDWINDOW,
//        CW_USEDEFAULT, 
//        CW_USEDEFAULT, 
//        width, 
//        height, 
//        nullptr, 
//        nullptr, 
//        hInstance,
//        nullptr
//    );
//    // TODO: Change to CreateWindowExW(WS_EX_TOPMOST, L"MazeDXWindowClass", g_szAppName, WS_POPUP,
//    // to default to fullscreen.
//
//    if (!hwnd)
//    {
//        MessageBox(NULL, L"hwnd is not valid", L"hwnd", MB_OK);
//        return 1;
//    }
//    DirectX11Instance = new DirectX11(hInstance, hwnd, width, height);
//    Application = new App(*DirectX11Instance);
//
//    ShowWindow(hwnd, nCmdShow);
//    // TODO: Change nCmdShow to SW_SHOWMAXIMIZED to default to fullscreen.
//
//    GetClientRect(hwnd, &rc);
//
//    // Main message loop
//    MSG msg = {};
//    while (WM_QUIT != msg.message)
//    {
//        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//        else
//        {
//            Application->Tick(*DirectX11Instance);
//        }
//    }
//
//    Util::SafeDelete(DirectX11Instance);
//    Util::SafeDelete(Application);
//
//    CoUninitialize();
//
//    return static_cast<int>(msg.wParam);
//}
//
//// Windows procedure
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    static bool s_in_sizemove = false;
//    static bool s_in_suspend = false;
//    static bool s_minimized = false;
//    static bool s_fullscreen = false;
//    // TODO: Set s_fullscreen to true if defaulting to fullscreen.
//
//    if (!Application)
//    {
//        return DefWindowProc(hWnd, message, wParam, lParam);;
//    }
//    if (!DirectX11Instance->bInitialized)
//    {
//        return DefWindowProc(hWnd, message, wParam, lParam);;
//    }
//    switch (message)
//    {
//    case WM_PAINT:
//        if (s_in_sizemove && Application)
//        {
//            Application->Tick(*DirectX11Instance);
//        }
//        else
//        {
//            PAINTSTRUCT ps;
//            std::ignore = BeginPaint(hWnd, &ps);
//            EndPaint(hWnd, &ps);
//        }
//        break;
//
//    case WM_SIZE:
//        if (wParam == SIZE_MINIMIZED)
//        {
//            if (!s_minimized)
//            {
//                s_minimized = true;
//                if (!s_in_suspend && Application)
//                {
//                    Application->OnSuspending();
//                }
//                s_in_suspend = true;
//            }
//        }
//        else if (s_minimized)
//        {
//            s_minimized = false;
//            if (s_in_suspend && Application)
//            {
//                Application->OnResuming();
//            }
//            s_in_suspend = false;
//        }
//        else if (!s_in_sizemove && Application)
//        {
//            Application->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
//        }
//        break;
//
//    case WM_ENTERSIZEMOVE:
//        s_in_sizemove = true;
//        break;
//
//    case WM_EXITSIZEMOVE:
//        s_in_sizemove = false;
//        if (Application)
//        {
//            RECT rc;
//            GetClientRect(hWnd, &rc);
//
//            Application->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
//        }
//        break;
//
//    case WM_GETMINMAXINFO:
//        if (lParam)
//        {
//            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
//            info->ptMinTrackSize.x = 320;
//            info->ptMinTrackSize.y = 200;
//        }
//        break;
//
//    case WM_ACTIVATEAPP:
//        if (Application)
//        {
//            if (wParam)
//            {
//                Application->OnActivated();
//            }
//            else
//            {
//                Application->OnDeactivated();
//            }
//        }
//        break;
//
//    case WM_POWERBROADCAST:
//        switch (wParam)
//        {
//        case PBT_APMQUERYSUSPEND:
//            if (!s_in_suspend && Application)
//            {
//                Application->OnSuspending();
//            }
//            s_in_suspend = true;
//            return TRUE;
//
//        case PBT_APMRESUMESUSPEND:
//            if (!s_minimized)
//            {
//                if (s_in_suspend && Application)
//                {
//                    Application->OnResuming();
//                }
//                s_in_suspend = false;
//            }
//            return TRUE;
//        }
//        break;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//
//    case WM_SYSKEYDOWN:
//        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
//        {
//            // Implements the classic ALT+ENTER fullscreen toggle
//            if (s_fullscreen)
//            {
//                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
//                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);
//
//                int width = 800;
//                int height = 600;
//                if (Application)
//                {
//                    Application->GetDefaultSize(width, height);
//                }
//
//                ShowWindow(hWnd, SW_SHOWNORMAL);
//
//                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
//            }
//            else
//            {
//                SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
//                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
//
//                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
//
//                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
//            }
//
//            s_fullscreen = !s_fullscreen;
//        }
//        break;
//
//    case WM_MENUCHAR:
//        // A menu is active and the user presses a key that does not correspond
//        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
//        return MAKELRESULT(0, MNC_CLOSE);
//    }
//
//    return DefWindowProc(hWnd, message, wParam, lParam);
//}

// Exit helper
void ExitGame() noexcept
{
    PostQuitMessage(0);
}


//
////Include and link appropriate libraries and headers//
//#pragma comment(lib, "d3d11.lib")
//#pragma comment (lib, "D3D10_1.lib")
//#pragma comment (lib, "DXGI.lib")
//#pragma comment (lib, "D2D1.lib")
//#pragma comment (lib, "dwrite.lib")
//#pragma comment (lib, "dinput8.lib")
//#pragma comment (lib, "dxguid.lib")
//#pragma comment(lib, "d3dcompiler.lib")
//
//#include "DirectX/DirectXHead.h"
//#include "DirectX/WICTextureLoader/WICTextureLoader11.h"
//#include "DirectX/DDSTextureLoader/DDSTextureLoader11.h"
//#include "Utils/String.h"
//
//#include <windows.h>
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include <D3D10_1.h>
//#include <DXGI.h>
//#include <D2D1.h>
//#include <sstream>
//#include <dwrite.h>
//#include <dinput.h>
/////////////////**************new**************////////////////////
//#include <vector>
/////////////////**************new**************////////////////////
//
//using namespace DirectX;
//
////Global Declarations - Interfaces//
//IDXGISwapChain* SwapChain;
//ID3D11Device* d3d11Device;
//ID3D11DeviceContext* d3d11DevCon;
//ID3D11RenderTargetView* renderTargetView;
//ID3D11Buffer* squareIndexBuffer;
//ID3D11DepthStencilView* depthStencilView;
//ID3D11Texture2D* depthStencilBuffer;
//ID3D11Buffer* squareVertBuffer;
//ID3D11VertexShader* VS;
//ID3D11PixelShader* PS;
//ID3D11PixelShader* D2D_PS;
//ID3D10Blob* D2D_PS_Buffer;
//ID3D10Blob* VS_Buffer;
//ID3D10Blob* PS_Buffer;
//ID3D11InputLayout* vertLayout;
//ID3D11Buffer* cbPerObjectBuffer;
//ID3D11BlendState* Transparency;
//ID3D11RasterizerState* CCWcullMode;
//ID3D11RasterizerState* CWcullMode;
//ID3D11ShaderResourceView* CubesTexture;
//ID3D11SamplerState* CubesTexSamplerState;
//ID3D11Buffer* cbPerFrameBuffer;
//
//ID3D10Device1* d3d101Device;
//IDXGIKeyedMutex* keyedMutex11;
//IDXGIKeyedMutex* keyedMutex10;
//ID2D1RenderTarget* D2DRenderTarget;
//ID2D1SolidColorBrush* Brush;
//ID3D11Texture2D* BackBuffer11;
//ID3D11Texture2D* sharedTex11;
//ID3D11Buffer* d2dVertBuffer;
//ID3D11Buffer* d2dIndexBuffer;
//ID3D11ShaderResourceView* d2dTexture;
//IDWriteFactory* DWriteFactory;
//IDWriteTextFormat* TextFormat;
//
//IDirectInputDevice8* DIKeyboard;
//IDirectInputDevice8* DIMouse;
//
/////////////////**************new**************////////////////////
//ID3D11Buffer* sphereIndexBuffer;
//ID3D11Buffer* sphereVertBuffer;
//
//ID3D11VertexShader* SKYMAP_VS;
//ID3D11PixelShader* SKYMAP_PS;
//ID3D10Blob* SKYMAP_VS_Buffer;
//ID3D10Blob* SKYMAP_PS_Buffer;
//
//ID3D11ShaderResourceView* smrv;
//
//ID3D11DepthStencilState* DSLessEqual;
//ID3D11RasterizerState* RSCullNone;
/////////////////**************new**************////////////////////
//
//std::wstring printText;
//
////Global Declarations - Others//
//LPCTSTR WndClassName = L"firstwindow";
//HWND m_hwnd = NULL;
//HRESULT hr;
//
//int Width = 800;
//int Height = 600;
//
//DIMOUSESTATE mouseLastState;
//LPDIRECTINPUT8 DirectInput;
//
//float rotx = 0;
//float rotz = 0;
//float scaleX = 1.0f;
//float scaleY = 1.0f;
//
//XMMATRIX Rotationx;
//XMMATRIX Rotationy;
//XMMATRIX Rotationz;
//
//XMMATRIX WVP;
//XMMATRIX cube1World;
//XMMATRIX cube2World;
//XMMATRIX camView;
//XMMATRIX camProjection;
//
//XMMATRIX d2dWorld;
//
//XMVECTOR camPosition;
//XMVECTOR camTarget;
//XMVECTOR camUp;
//XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//
//XMMATRIX camRotationMatrix;
//XMMATRIX groundWorld;
//
//float moveLeftRight = 0.0f;
//float moveBackForward = 0.0f;
//
//float camYaw = 0.0f;
//float camPitch = 0.0f;
//
/////////////////**************new**************////////////////////
//int NumSphereVertices;
//int NumSphereFaces;
//
//XMMATRIX sphereWorld;
/////////////////**************new**************////////////////////
//
//XMMATRIX Rotation;
//XMMATRIX Scale;
//XMMATRIX Translation;
//float rot = 0.01f;
//
//double countsPerSecond = 0.0;
//__int64 CounterStart = 0;
//
//int frameCount = 0;
//int fps = 0;
//
//__int64 frameTimeOld = 0;
//double frameTime;
//
////Function Prototypes//
//bool InitializeDirect3d11App(HINSTANCE hInstance);
//void CleanUp();
//bool InitScene();
//void DrawScene();
//bool InitD2D_D3D101_DWrite(IDXGIAdapter1* Adapter);
//void InitD2DScreenTexture();
//void UpdateScene(double time);
//
//void UpdateCamera();
/////////////////**************new**************////////////////////
//void CreateSphere(int LatLines, int LongLines);
/////////////////**************new**************////////////////////
//
//void RenderText(std::wstring text, int inInt);
//
//void StartTimer();
//double GetTime();
//double GetFrameTime();
//
//bool InitializeWindow(HINSTANCE hInstance,
//	int ShowWnd,
//	int width, int height,
//	bool windowed);
//int messageloop();
//
//bool InitDirectInput(HINSTANCE hInstance);
//void DetectInput(double time);
//
//LRESULT CALLBACK WndProc(HWND hWnd,
//	UINT msg,
//	WPARAM wParam,
//	LPARAM lParam);
//
////Create effects constant buffer's structure//
//struct cbPerObject
//{
//	XMMATRIX  WVP;
//	XMMATRIX World;
//};
//
//cbPerObject cbPerObj;
//
//struct Light
//{
//	Light()
//	{
//		ZeroMemory(this, sizeof(Light));
//	}
//	XMFLOAT3 dir;
//	float pad;
//	XMFLOAT4 ambient;
//	XMFLOAT4 diffuse;
//};
//
//Light light;
//
//struct cbPerFrame
//{
//	Light  light;
//};
//
//cbPerFrame constbuffPerFrame;
//
//struct Vertex	//Overloaded Vertex Structure
//{
//	Vertex() {}
//	Vertex(float x, float y, float z,
//		float u, float v,
//		float nx, float ny, float nz)
//		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}
//
//	XMFLOAT3 pos;
//	XMFLOAT2 texCoord;
//	XMFLOAT3 normal;
//};
//
//D3D11_INPUT_ELEMENT_DESC layout[] =
//{
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
//};
//UINT numElements = ARRAYSIZE(layout);
//
//int WINAPI WinMain(HINSTANCE hInstance,	//Main windows function
//	HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine,
//	int nShowCmd)
//{
//
//	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
//	{
//		MessageBox(0, L"Window Initialization - Failed",
//			L"Error", MB_OK);
//		return 0;
//	}
//
//	if (!InitializeDirect3d11App(hInstance))	//Initialize Direct3D
//	{
//		MessageBox(0, L"Direct3D Initialization - Failed",
//			L"Error", MB_OK);
//		return 0;
//	}
//
//	if (!InitScene())	//Initialize our scene
//	{
//		MessageBox(0, L"Scene Initialization - Failed",
//			L"Error", MB_OK);
//		return 0;
//	}
//
//	if (!InitDirectInput(hInstance))
//	{
//		MessageBox(0, L"Direct Input Initialization - Failed",
//			L"Error", MB_OK);
//		return 0;
//	}
//
//	messageloop();
//
//	CleanUp();
//
//	return 0;
//}
//
//bool InitializeWindow(HINSTANCE hInstance,
//	int ShowWnd,
//	int width, int height,
//	bool windowed)
//{
//	typedef struct _WNDCLASS {
//		UINT cbSize;
//		UINT style;
//		WNDPROC lpfnWndProc;
//		int cbClsExtra;
//		int cbWndExtra;
//		HANDLE hInstance;
//		HICON hIcon;
//		HCURSOR hCursor;
//		HBRUSH hbrBackground;
//		LPCTSTR lpszMenuName;
//		LPCTSTR lpszClassName;
//	};
//
//	WNDCLASSEX wc;
//
//	wc.cbSize = sizeof(WNDCLASSEX);
//	wc.style = CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = NULL;
//	wc.cbWndExtra = NULL;
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = WndClassName;
//	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//
//	if (!RegisterClassEx(&wc))
//	{
//		MessageBox(NULL, L"Error registering class",
//			L"Error", MB_OK | MB_ICONERROR);
//		return 1;
//	}
//
//	m_hwnd = CreateWindowEx(
//		NULL,
//		WndClassName,
//		L"Lesson 4 - Begin Drawing",
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT, CW_USEDEFAULT,
//		width, height,
//		NULL,
//		NULL,
//		hInstance,
//		NULL
//	);
//
//	if (!m_hwnd)
//	{
//		MessageBox(NULL, L"Error creating window",
//			L"Error", MB_OK | MB_ICONERROR);
//		return 1;
//	}
//
//	ShowWindow(m_hwnd, ShowWnd);
//	UpdateWindow(m_hwnd);
//
//	return true;
//}
//
//bool InitializeDirect3d11App(HINSTANCE hInstance)
//{
//	//Describe our SwapChain Buffer
//	DXGI_MODE_DESC bufferDesc;
//
//	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
//
//	bufferDesc.Width = Width;
//	bufferDesc.Height = Height;
//	bufferDesc.RefreshRate.Numerator = 60;
//	bufferDesc.RefreshRate.Denominator = 1;
//	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
//	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//
//	//Describe our SwapChain
//	DXGI_SWAP_CHAIN_DESC swapChainDesc;
//
//	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
//
//	swapChainDesc.BufferDesc = bufferDesc;
//	swapChainDesc.SampleDesc.Count = 1;
//	swapChainDesc.SampleDesc.Quality = 0;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.OutputWindow = m_hwnd;
//	swapChainDesc.Windowed = true;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//
//	// Create DXGI factory to enumerate adapters///////////////////////////////////////////////////////////////////////////
//	IDXGIFactory1* DXGIFactory;
//
//	hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGIFactory);
//
//	// Use the first adapter	
//	IDXGIAdapter1* Adapter;
//
//	hr = DXGIFactory->EnumAdapters1(0, &Adapter);
//
//	DXGIFactory->Release();
//
//	//Create our Direct3D 11 Device and SwapChain//////////////////////////////////////////////////////////////////////////
//	hr = D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
//		NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);
//
//	//Initialize Direct2D, Direct3D 10.1, DirectWrite
//	InitD2D_D3D101_DWrite(Adapter);
//
//	//Release the Adapter interface
//	Adapter->Release();
//
//	//Create our BackBuffer and Render Target
//	hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer11);
//	hr = d3d11Device->CreateRenderTargetView(BackBuffer11, NULL, &renderTargetView);
//
//	//Describe our Depth/Stencil Buffer
//	D3D11_TEXTURE2D_DESC depthStencilDesc;
//
//	depthStencilDesc.Width = Width;
//	depthStencilDesc.Height = Height;
//	depthStencilDesc.MipLevels = 1;
//	depthStencilDesc.ArraySize = 1;
//	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	depthStencilDesc.SampleDesc.Count = 1;
//	depthStencilDesc.SampleDesc.Quality = 0;
//	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
//	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	depthStencilDesc.CPUAccessFlags = 0;
//	depthStencilDesc.MiscFlags = 0;
//
//	//Create the Depth/Stencil View
//	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
//	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);
//
//	return true;
//}
//
//bool InitD2D_D3D101_DWrite(IDXGIAdapter1* Adapter)
//{
//	//Create our Direc3D 10.1 Device///////////////////////////////////////////////////////////////////////////////////////
//	hr = D3D10CreateDevice1(Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_BGRA_SUPPORT,
//		D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device);
//
//	//Create Shared Texture that Direct3D 10.1 will render on//////////////////////////////////////////////////////////////
//	D3D11_TEXTURE2D_DESC sharedTexDesc;
//
//	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));
//
//	sharedTexDesc.Width = Width;
//	sharedTexDesc.Height = Height;
//	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	sharedTexDesc.MipLevels = 1;
//	sharedTexDesc.ArraySize = 1;
//	sharedTexDesc.SampleDesc.Count = 1;
//	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
//	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
//
//	hr = d3d11Device->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
//
//	// Get the keyed mutex for the shared texture (for D3D11)///////////////////////////////////////////////////////////////
//	hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex11);
//
//	// Get the shared handle needed to open the shared texture in D3D10.1///////////////////////////////////////////////////
//	IDXGIResource* sharedResource10;
//	HANDLE sharedHandle10;
//
//	hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10);
//
//	hr = sharedResource10->GetSharedHandle(&sharedHandle10);
//
//	sharedResource10->Release();
//
//	// Open the surface for the shared texture in D3D10.1///////////////////////////////////////////////////////////////////
//	IDXGISurface1* sharedSurface10;
//
//	hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10));
//
//	hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&keyedMutex10);
//
//	// Create D2D factory///////////////////////////////////////////////////////////////////////////////////////////////////
//	ID2D1Factory* D2DFactory;
//	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory);
//
//	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
//
//	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));
//
//	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
//	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
//
//	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);
//
//	sharedSurface10->Release();
//	D2DFactory->Release();
//
//	// Create a solid color brush to draw something with		
//	hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &Brush);
//
//	//DirectWrite///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
//		reinterpret_cast<IUnknown**>(&DWriteFactory));
//
//	hr = DWriteFactory->CreateTextFormat(
//		L"Script",
//		NULL,
//		DWRITE_FONT_WEIGHT_REGULAR,
//		DWRITE_FONT_STYLE_NORMAL,
//		DWRITE_FONT_STRETCH_NORMAL,
//		24.0f,
//		L"en-us",
//		&TextFormat
//	);
//
//	hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
//	hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
//
//	d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
//	return true;
//}
//
//bool InitDirectInput(HINSTANCE hInstance)
//{
//	hr = DirectInput8Create(hInstance,
//		DIRECTINPUT_VERSION,
//		IID_IDirectInput8,
//		(void**)&DirectInput,
//		NULL);
//
//	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
//		&DIKeyboard,
//		NULL);
//
//	hr = DirectInput->CreateDevice(GUID_SysMouse,
//		&DIMouse,
//		NULL);
//
//	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
//	hr = DIKeyboard->SetCooperativeLevel(m_hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//
//	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
//	hr = DIMouse->SetCooperativeLevel(m_hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
//
//	return true;
//}
//
//void UpdateCamera()
//{
//	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
//	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
//	camTarget = XMVector3Normalize(camTarget);
//
//	XMMATRIX RotateYTempMatrix;
//	RotateYTempMatrix = XMMatrixRotationY(camYaw);
//
//	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
//	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
//	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
//
//	camPosition += moveLeftRight * camRight;
//	camPosition += moveBackForward * camForward;
//
//	moveLeftRight = 0.0f;
//	moveBackForward = 0.0f;
//
//	camTarget = camPosition + camTarget;
//
//	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
//}
//
//void DetectInput(double time)
//{
//	DIMOUSESTATE mouseCurrState;
//
//	BYTE keyboardState[256];
//
//	DIKeyboard->Acquire();
//	DIMouse->Acquire();
//
//	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
//
//	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
//
//	if (keyboardState[DIK_ESCAPE] & 0x80)
//		PostMessage(m_hwnd, WM_DESTROY, 0, 0);
//
//	float speed = 15.0f * time;
//
//	if (keyboardState[DIK_A] & 0x80)
//	{
//		moveLeftRight -= speed;
//	}
//	if (keyboardState[DIK_D] & 0x80)
//	{
//		moveLeftRight += speed;
//	}
//	if (keyboardState[DIK_W] & 0x80)
//	{
//		moveBackForward += speed;
//	}
//	if (keyboardState[DIK_S] & 0x80)
//	{
//		moveBackForward -= speed;
//	}
//	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
//	{
//		camYaw += mouseLastState.lX * 0.001f;
//
//		camPitch += mouseCurrState.lY * 0.001f;
//
//		mouseLastState = mouseCurrState;
//	}
//
//	UpdateCamera();
//
//	return;
//}
//
//void CleanUp()
//{
//	SwapChain->SetFullscreenState(false, NULL);
//	PostMessage(m_hwnd, WM_DESTROY, 0, 0);
//
//	//Release the COM Objects we created
//	SwapChain->Release();
//	d3d11Device->Release();
//	d3d11DevCon->Release();
//	renderTargetView->Release();
//	squareVertBuffer->Release();
//	squareIndexBuffer->Release();
//	VS->Release();
//	PS->Release();
//	VS_Buffer->Release();
//	PS_Buffer->Release();
//	vertLayout->Release();
//	depthStencilView->Release();
//	depthStencilBuffer->Release();
//	cbPerObjectBuffer->Release();
//	Transparency->Release();
//	CCWcullMode->Release();
//	CWcullMode->Release();
//
//	d3d101Device->Release();
//	keyedMutex11->Release();
//	keyedMutex10->Release();
//	D2DRenderTarget->Release();
//	Brush->Release();
//	BackBuffer11->Release();
//	sharedTex11->Release();
//	DWriteFactory->Release();
//	TextFormat->Release();
//	d2dTexture->Release();
//
//	cbPerFrameBuffer->Release();
//
//	DIKeyboard->Unacquire();
//	DIMouse->Unacquire();
//	DirectInput->Release();
//
//	///////////////**************new**************////////////////////
//	sphereIndexBuffer->Release();
//	sphereVertBuffer->Release();
//
//	SKYMAP_VS->Release();
//	SKYMAP_PS->Release();
//	SKYMAP_VS_Buffer->Release();
//	SKYMAP_PS_Buffer->Release();
//
//	smrv->Release();
//
//	DSLessEqual->Release();
//	RSCullNone->Release();
//	///////////////**************new**************////////////////////
//}
//
/////////////////**************new**************////////////////////
//void CreateSphere(int LatLines, int LongLines)
//{
//	NumSphereVertices = ((LatLines - 2) * LongLines) + 2;
//	NumSphereFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);
//
//	float sphereYaw = 0.0f;
//	float spherePitch = 0.0f;
//
//	std::vector<Vertex> vertices(NumSphereVertices);
//
//	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
//
//	vertices[0].pos.x = 0.0f;
//	vertices[0].pos.y = 0.0f;
//	vertices[0].pos.z = 1.0f;
//
//	for (DWORD i = 0; i < LatLines - 2; ++i)
//	{
//		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
//		Rotationx = XMMatrixRotationX(spherePitch);
//		for (DWORD j = 0; j < LongLines; ++j)
//		{
//			sphereYaw = j * (6.28 / (LongLines));
//			Rotationy = XMMatrixRotationZ(sphereYaw);
//			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
//			currVertPos = XMVector3Normalize(currVertPos);
//			vertices[i * LongLines + j + 1].pos.x = XMVectorGetX(currVertPos);
//			vertices[i * LongLines + j + 1].pos.y = XMVectorGetY(currVertPos);
//			vertices[i * LongLines + j + 1].pos.z = XMVectorGetZ(currVertPos);
//		}
//	}
//
//	vertices[NumSphereVertices - 1].pos.x = 0.0f;
//	vertices[NumSphereVertices - 1].pos.y = 0.0f;
//	vertices[NumSphereVertices - 1].pos.z = -1.0f;
//
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
//
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * NumSphereVertices;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexBufferData;
//
//	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
//	vertexBufferData.pSysMem = &vertices[0];
//	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);
//
//
//	std::vector<DWORD> indices(NumSphereFaces * 3);
//
//	int k = 0;
//	for (DWORD l = 0; l < LongLines - 1; ++l)
//	{
//		indices[k] = 0;
//		indices[k + 1] = l + 1;
//		indices[k + 2] = l + 2;
//		k += 3;
//	}
//
//	indices[k] = 0;
//	indices[k + 1] = LongLines;
//	indices[k + 2] = 1;
//	k += 3;
//
//	for (DWORD i = 0; i < LatLines - 3; ++i)
//	{
//		for (DWORD j = 0; j < LongLines - 1; ++j)
//		{
//			indices[k] = i * LongLines + j + 1;
//			indices[k + 1] = i * LongLines + j + 2;
//			indices[k + 2] = (i + 1) * LongLines + j + 1;
//
//			indices[k + 3] = (i + 1) * LongLines + j + 1;
//			indices[k + 4] = i * LongLines + j + 2;
//			indices[k + 5] = (i + 1) * LongLines + j + 2;
//
//			k += 6; // next quad
//		}
//
//		indices[k] = (i * LongLines) + LongLines;
//		indices[k + 1] = (i * LongLines) + 1;
//		indices[k + 2] = ((i + 1) * LongLines) + LongLines;
//
//		indices[k + 3] = ((i + 1) * LongLines) + LongLines;
//		indices[k + 4] = (i * LongLines) + 1;
//		indices[k + 5] = ((i + 1) * LongLines) + 1;
//
//		k += 6;
//	}
//
//	for (DWORD l = 0; l < LongLines - 1; ++l)
//	{
//		indices[k] = NumSphereVertices - 1;
//		indices[k + 1] = (NumSphereVertices - 1) - (l + 1);
//		indices[k + 2] = (NumSphereVertices - 1) - (l + 2);
//		k += 3;
//	}
//
//	indices[k] = NumSphereVertices - 1;
//	indices[k + 1] = (NumSphereVertices - 1) - LongLines;
//	indices[k + 2] = NumSphereVertices - 2;
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
//
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//
//	iinitData.pSysMem = &indices[0];
//	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);
//
//}
/////////////////**************new**************////////////////////
//
//void InitD2DScreenTexture()
//{
//	//Create the vertex buffer
//	Vertex v[] =
//	{
//		// Front Face
//		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f),
//		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f),
//		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f),
//		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f),
//	};
//
//	DWORD indices[] = {
//		// Front Face
//		0,  1,  2,
//		0,  2,  3,
//	};
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
//
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//
//	iinitData.pSysMem = indices;
//	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
//
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexBufferData;
//
//	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
//	vertexBufferData.pSysMem = v;
//	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);
//
//	//Create A shader resource view from the texture D2D will render to,
//	//So we can use it to texture a square which overlays our scene
//	d3d11Device->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);
//}
//
//bool Compile(std::wstring pFilename, const char* pEntryPoint, const char* pShaderModel, ID3DBlob** ppBlob)
//{
//	WCHAR path[256];
//	size_t len = 0;
//
//	size_t outputSize = pFilename.length() + 1;
//	char* outputStr = new char[outputSize];
//
//	size_t charsConverted = 0;
//
//	wcstombs_s(&charsConverted, outputStr, outputSize, pFilename.c_str(), pFilename.length());
//	mbstowcs_s(&len, path, 256, outputStr, _TRUNCATE);
//
//	ID3DBlob* pErrorMsg;
//	hr = D3DCompileFromFile(
//		path,
//		NULL,
//		D3D_COMPILE_STANDARD_FILE_INCLUDE,
//		pEntryPoint,
//		pShaderModel,
//		0,
//		0,
//		ppBlob,
//		&pErrorMsg
//	);
//	if (FAILED(hr))
//	{
//		static_cast<char*>(pErrorMsg->GetBufferPointer());
//		MessageBox(NULL, L"Compile Failed", L"Shader Compile Error", MB_OK);
//		Util::SafeRelease(pErrorMsg);
//		assert(false);
//		return false;
//	}
//
//	Util::SafeRelease(pErrorMsg);
//	return true;
//}
//bool InitScene()
//{
//	InitD2DScreenTexture();
//
//	///////////////**************new**************////////////////////
//	CreateSphere(10, 10);
//	///////////////**************new**************////////////////////
//
//	//Compile Shaders from shader file
//	hr = Compile(L"Shader/Effects.fx", "VS", "vs_4_0", &VS_Buffer);
//	hr = Compile(L"Shader/Effects.fx", "PS", "ps_4_0", &PS_Buffer);
//	hr = Compile(L"Shader/Effects.fx", "D2D_PS", "ps_4_0", &D2D_PS_Buffer);
//	///////////////**************new**************////////////////////
//	hr = Compile(L"Shader/Effects.fx", "SKYMAP_VS", "vs_4_0", &SKYMAP_VS_Buffer);
//	hr = Compile(L"Shader/Effects.fx", "SKYMAP_PS", "ps_4_0", &SKYMAP_PS_Buffer);
//	///////////////**************new**************////////////////////
//
//	//Create the Shader Objects
//	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
//	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
//	hr = d3d11Device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
//	///////////////**************new**************////////////////////
//	hr = d3d11Device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
//	hr = d3d11Device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
//	///////////////**************new**************////////////////////
//
//	//Set Vertex and Pixel Shaders
//	d3d11DevCon->VSSetShader(VS, 0, 0);
//	d3d11DevCon->PSSetShader(PS, 0, 0);
//
//	light.dir = XMFLOAT3(0.0f, 1.0f, 0.0f);
//	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
//	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//
//	//Create the vertex buffer
//	Vertex v[] =
//	{
//		// Bottom Face
//		Vertex(-1.0f, -1.0f, -1.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f),
//		Vertex(1.0f, -1.0f, -1.0f,   0.0f, 100.0f, 0.0f, 1.0f, 0.0f),
//		Vertex(1.0f, -1.0f,  1.0f,   0.0f,   0.0f, 0.0f, 1.0f, 0.0f),
//		Vertex(-1.0f, -1.0f,  1.0f, 100.0f,   0.0f, 0.0f, 1.0f, 0.0f),
//	};
//
//	DWORD indices[] = {
//		0,  1,  2,
//		0,  2,  3,
//	};
//	std::vector<DWORD> ind = {
//		// Front Face
//		0,  1,  2,
//		0,  2,  3,
//	};
//
//	D3D11_BUFFER_DESC indexBufferDesc;
//	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
//
//	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
//	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	indexBufferDesc.CPUAccessFlags = 0;
//	indexBufferDesc.MiscFlags = 0;
//
//	std::string str = std::string("index buffer : ") + std::to_string(indexBufferDesc.ByteWidth) + std::string(" : ") + std::to_string(ind.size() * sizeof(DWORD));
//	MessageBox(NULL, Util::s2WString(str).c_str(), L"Shader Compile Error", MB_OK);
//
//	D3D11_SUBRESOURCE_DATA iinitData;
//
//	iinitData.pSysMem = indices;
//	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);
//
//	D3D11_BUFFER_DESC vertexBufferDesc;
//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
//
//	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	vertexBufferDesc.CPUAccessFlags = 0;
//	vertexBufferDesc.MiscFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA vertexBufferData;
//
//	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
//	vertexBufferData.pSysMem = v;
//	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);
//
//	str = std::string("Vertex buffer : ") + std::to_string(vertexBufferDesc.ByteWidth) + std::string(" : ") + std::to_string(4 * sizeof(Vertex));
//	MessageBox(NULL, Util::s2WString(str).c_str(), L"Shader Compile Error", MB_OK);
//
//	//Create the Input Layout
//	hr = d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
//		VS_Buffer->GetBufferSize(), &vertLayout);
//
//	//Set the Input Layout
//	d3d11DevCon->IASetInputLayout(vertLayout);
//
//	//Set Primitive Topology
//	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//Create the Viewport
//	D3D11_VIEWPORT viewport;
//	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
//
//	viewport.TopLeftX = 0;
//	viewport.TopLeftY = 0;
//	viewport.Width = Width;
//	viewport.Height = Height;
//	viewport.MinDepth = 0.0f;
//	viewport.MaxDepth = 1.0f;
//
//	//Set the Viewport
//	d3d11DevCon->RSSetViewports(1, &viewport);
//
//	//Create the buffer to send to the cbuffer in effect file
//	D3D11_BUFFER_DESC cbbd;
//	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
//
//	cbbd.Usage = D3D11_USAGE_DEFAULT;
//	cbbd.ByteWidth = sizeof(cbPerObject);
//	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cbbd.CPUAccessFlags = 0;
//	cbbd.MiscFlags = 0;
//
//	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);
//
//	//Create the buffer to send to the cbuffer per frame in effect file
//	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
//
//	cbbd.Usage = D3D11_USAGE_DEFAULT;
//	cbbd.ByteWidth = sizeof(cbPerFrame);
//	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	cbbd.CPUAccessFlags = 0;
//	cbbd.MiscFlags = 0;
//
//	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);
//
//	//Camera information
//	camPosition = XMVectorSet(0.0f, 5.0f, -8.0f, 0.0f);
//	camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
//	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//	//Set the View matrix
//	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
//
//	//Set the Projection matrix
//	camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, Width / Height, 1.0f, 1000.0f);
//
//	D3D11_BLEND_DESC blendDesc;
//	ZeroMemory(&blendDesc, sizeof(blendDesc));
//
//	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
//	ZeroMemory(&rtbd, sizeof(rtbd));
//
//	rtbd.BlendEnable = true;
//	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
//	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
//	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
//	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
//	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
//	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
//
//	blendDesc.AlphaToCoverageEnable = false;
//	blendDesc.RenderTarget[0] = rtbd;
//
//	ScratchImage scratch;
//	std::wstring inFileName = L"grass.jpg";
//	DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	HRESULT result = DirectX::LoadFromWICFile(
//		inFileName.c_str(),
//		DirectX::WIC_FLAGS_IGNORE_SRGB,
//		nullptr,
//		scratch
//	);
//	if (FAILED(result))
//	{
//		MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
//		assert(false);
//	}
//
//	if (scratch.GetImage(0, 0, 0)->format != format)
//	{
//		DirectX::ScratchImage converted;
//		result = DirectX::Convert(
//			*scratch.GetImage(0, 0, 0),
//			format,
//			DirectX::TEX_FILTER_DEFAULT,
//			0.5,
//			converted
//		);
//		if (FAILED(result))
//		{
//			MessageBox(NULL, L"Can not load Convert Texture", L"Failed Texture constructor", MB_OK);
//			assert(false);
//		}
//		scratch = std::move(converted);
//	}
//
//	UINT width = (UINT)scratch.GetImage(0, 0, 0)->width;
//	UINT height = (UINT)scratch.GetImage(0, 0, 0)->height;
//	D3D11_TEXTURE2D_DESC texDesc;
//	ZeroMemory(&texDesc, sizeof(texDesc));
//	texDesc.Width = width;
//	texDesc.Height = height;
//	texDesc.MipLevels = 0;
//	texDesc.ArraySize = 1;
//	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.SampleDesc.Quality = 0;
//	texDesc.Usage = D3D11_USAGE_DEFAULT;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//	texDesc.CPUAccessFlags = 0;
//	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//
//	ID3D11Texture2D* pTexture;
//	d3d11Device->CreateTexture2D(&texDesc, nullptr, &pTexture);
//
//	d3d11DevCon->UpdateSubresource(
//		pTexture,
//		0,
//		nullptr,
//		scratch.GetPixels(),
//		width * sizeof(DWORD),
//		0
//	);
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//	ZeroMemory(&srvDesc, sizeof(srvDesc));
//	srvDesc.Format = texDesc.Format;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MostDetailedMip = 0;
//	srvDesc.Texture2D.MipLevels = 1;
//	d3d11Device->CreateShaderResourceView(pTexture, &srvDesc, &CubesTexture);
//
//	d3d11DevCon->GenerateMips(CubesTexture);
//
//	///////////////**************new**************////////////////////
//	//Tell D3D we will be loading a cube texture
//	inFileName = L"skymap.dds";
//	format = DXGI_FORMAT_B8G8R8A8_UNORM;
//	result = DirectX::LoadFromDDSFile(
//		inFileName.c_str(),
//		DirectX::DDS_FLAGS_ALLOW_LARGE_FILES,
//		nullptr,
//		scratch
//	);
//	if (FAILED(result))
//	{
//		MessageBox(NULL, L"Can not load Texture", L"Failed Texture constructor", MB_OK);
//		assert(false);
//	}
//
//	if (scratch.GetImage(0, 0, 0)->format != format)
//	{
//		DirectX::ScratchImage converted;
//		result = DirectX::Convert(
//			*scratch.GetImage(0, 0, 0),
//			format,
//			DirectX::TEX_FILTER_DEFAULT,
//			0.5,
//			converted
//		);
//		if (FAILED(result))
//		{
//			MessageBox(NULL, L"Can not load Convert Texture", L"Failed Texture constructor", MB_OK);
//			assert(false);
//		}
//		scratch = std::move(converted);
//	}
//
//	width = (UINT)scratch.GetImage(0, 0, 0)->width;
//	height = (UINT)scratch.GetImage(0, 0, 0)->height;
//	texDesc;
//	ZeroMemory(&texDesc, sizeof(texDesc));
//	texDesc.Width = width;
//	texDesc.Height = height;
//	texDesc.MipLevels = 0;
//	texDesc.ArraySize = 1;
//	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.SampleDesc.Quality = 0;
//	texDesc.Usage = D3D11_USAGE_DEFAULT;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
//	texDesc.CPUAccessFlags = 0;
//	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
//
//	ID3D11Texture2D* SMTexture;
//	d3d11Device->CreateTexture2D(&texDesc, nullptr, &SMTexture);
//
//	//Create the textures description
//	D3D11_TEXTURE2D_DESC SMTextureDesc;
//	SMTexture->GetDesc(&SMTextureDesc);
//
//	//Tell D3D We have a cube texture, which is an array of 2D textures
//	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
//	SMViewDesc.Format = SMTextureDesc.Format;
//	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
//	SMViewDesc.TextureCube.MostDetailedMip = 0;
//
//	//Create the Resource view
//	hr = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
//	///////////////**************new**************////////////////////
//
//	// Describe the Sample State
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	//Create the Sample State
//	hr = d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);
//
//	d3d11Device->CreateBlendState(&blendDesc, &Transparency);
//
//	D3D11_RASTERIZER_DESC cmdesc;
//
//	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
//	cmdesc.FillMode = D3D11_FILL_SOLID;
//	cmdesc.CullMode = D3D11_CULL_BACK;
//	cmdesc.FrontCounterClockwise = true;
//	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);
//
//	cmdesc.FrontCounterClockwise = false;
//
//	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);
//
//	///////////////**************new**************////////////////////
//	cmdesc.CullMode = D3D11_CULL_NONE;
//	hr = d3d11Device->CreateRasterizerState(&cmdesc, &RSCullNone);
//
//	D3D11_DEPTH_STENCIL_DESC dssDesc;
//	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
//	dssDesc.DepthEnable = true;
//	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
//
//	d3d11Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);
//	///////////////**************new**************////////////////////
//
//	return true;
//}
//
//void StartTimer()
//{
//	LARGE_INTEGER frequencyCount;
//	QueryPerformanceFrequency(&frequencyCount);
//
//	countsPerSecond = double(frequencyCount.QuadPart);
//
//	QueryPerformanceCounter(&frequencyCount);
//	CounterStart = frequencyCount.QuadPart;
//}
//
//double GetTime()
//{
//	LARGE_INTEGER currentTime;
//	QueryPerformanceCounter(&currentTime);
//	return double(currentTime.QuadPart - CounterStart) / countsPerSecond;
//}
//
//double GetFrameTime()
//{
//	LARGE_INTEGER currentTime;
//	__int64 tickCount;
//	QueryPerformanceCounter(&currentTime);
//
//	tickCount = currentTime.QuadPart - frameTimeOld;
//	frameTimeOld = currentTime.QuadPart;
//
//	if (tickCount < 0.0f)
//		tickCount = 0.0f;
//
//	return float(tickCount) / countsPerSecond;
//}
//
//void UpdateScene(double time)
//{
//	//Reset cube1World
//	groundWorld = XMMatrixIdentity();
//
//	//Define cube1's world space matrix
//	Scale = XMMatrixScaling(500.0f, 10.0f, 500.0f);
//	Translation = XMMatrixTranslation(0.0f, 10.0f, 0.0f);
//
//	//Set cube1's world space using the transformations
//	groundWorld = Scale * Translation;
//
//	///////////////**************new**************////////////////////
//	//Reset sphereWorld
//	sphereWorld = XMMatrixIdentity();
//
//	//Define sphereWorld's world space matrix
//	Scale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
//	//Make sure the sphere is always centered around camera
//	Translation = XMMatrixTranslation(XMVectorGetX(camPosition), XMVectorGetY(camPosition), XMVectorGetZ(camPosition));
//
//	//Set sphereWorld's world space using the transformations
//	sphereWorld = Scale * Translation;
//	///////////////**************new**************////////////////////
//}
//
//void RenderText(std::wstring text, int inInt)
//{
//
//	d3d11DevCon->PSSetShader(D2D_PS, 0, 0);
//
//	//Release the D3D 11 Device
//	keyedMutex11->ReleaseSync(0);
//
//	//Use D3D10.1 device
//	keyedMutex10->AcquireSync(0, 5);
//
//	//Draw D2D content		
//	D2DRenderTarget->BeginDraw();
//
//	//Clear D2D Background
//	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
//
//	//Create our string
//	std::wostringstream printString;
//	printString << text << inInt;
//	printText = printString.str();
//
//	//Set the Font Color
//	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
//
//	//Set the brush color D2D will use to draw with
//	Brush->SetColor(FontColor);
//
//	//Create the D2D Render Area
//	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, Width, Height);
//
//	//Draw the Text
//	D2DRenderTarget->DrawText(
//		printText.c_str(),
//		wcslen(printText.c_str()),
//		TextFormat,
//		layoutRect,
//		Brush
//	);
//
//	D2DRenderTarget->EndDraw();
//
//	//Release the D3D10.1 Device
//	keyedMutex10->ReleaseSync(1);
//
//	//Use the D3D11 Device
//	keyedMutex11->AcquireSync(1, 5);
//
//	//Use the shader resource representing the direct2d render target
//	//to texture a square which is rendered in screen space so it
//	//overlays on top of our entire scene. We use alpha blending so
//	//that the entire background of the D2D render target is "invisible",
//	//And only the stuff we draw with D2D will be visible (the text)
//
//	//Set the blend state for D2D render target texture objects
//	d3d11DevCon->OMSetBlendState(Transparency, NULL, 0xffffffff);
//
//	//Set the d2d Index buffer
//	d3d11DevCon->IASetIndexBuffer(d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	//Set the d2d vertex buffer
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	d3d11DevCon->IASetVertexBuffers(0, 1, &d2dVertBuffer, &stride, &offset);
//
//	WVP = XMMatrixIdentity();
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetShaderResources(0, 1, &d2dTexture);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	d3d11DevCon->RSSetState(CWcullMode);
//	//Draw the second cube
//	d3d11DevCon->DrawIndexed(6, 0, 0);
//}
//
//void DrawScene()
//{
//	//Clear our render target and depth/stencil view
//	float bgColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
//	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);
//	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	constbuffPerFrame.light = light;
//	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
//	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
//
//	//Set our Render Target
//	d3d11DevCon->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
//
//	//Set the default blend state (no blending) for opaque objects
//	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);
//
//	//Set Vertex and Pixel Shaders
//	d3d11DevCon->VSSetShader(VS, 0, 0);
//	d3d11DevCon->PSSetShader(PS, 0, 0);
//
//	//Set the grounds index buffer
//	d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	//Set the grounds vertex buffer
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = groundWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(groundWorld);
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	d3d11DevCon->PSSetShaderResources(0, 1, &CubesTexture);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	d3d11DevCon->RSSetState(CCWcullMode);
//	d3d11DevCon->DrawIndexed(6, 0, 0);
//
//	///////////////**************new**************////////////////////
//	//Set the spheres index buffer
//	d3d11DevCon->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	//Set the spheres vertex buffer
//	d3d11DevCon->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);
//
//	//Set the WVP matrix and send it to the constant buffer in effect file
//	WVP = sphereWorld * camView * camProjection;
//	cbPerObj.WVP = XMMatrixTranspose(WVP);
//	cbPerObj.World = XMMatrixTranspose(sphereWorld);
//	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
//	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
//	//Send our skymap resource view to pixel shader
//	d3d11DevCon->PSSetShaderResources(0, 1, &smrv);
//	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);
//
//	//Set the new VS and PS shaders
//	d3d11DevCon->VSSetShader(SKYMAP_VS, 0, 0);
//	d3d11DevCon->PSSetShader(SKYMAP_PS, 0, 0);
//	//Set the new depth/stencil and RS states
//	d3d11DevCon->OMSetDepthStencilState(DSLessEqual, 0);
//	d3d11DevCon->RSSetState(RSCullNone);
//	d3d11DevCon->DrawIndexed(NumSphereFaces * 3, 0, 0);
//
//	//Set the default VS shader and depth/stencil state
//	d3d11DevCon->VSSetShader(VS, 0, 0);
//	d3d11DevCon->OMSetDepthStencilState(NULL, 0);
//	///////////////**************new**************////////////////////
//
//	RenderText(L"FPS: ", fps);
//
//	//Present the backbuffer to the screen
//	SwapChain->Present(0, 0);
//}
//
//int messageloop() {
//	MSG msg;
//	ZeroMemory(&msg, sizeof(MSG));
//	while (true)
//	{
//		BOOL PeekMessageL(
//			LPMSG lpMsg,
//			HWND hWnd,
//			UINT wMsgFilterMin,
//			UINT wMsgFilterMax,
//			UINT wRemoveMsg
//		);
//
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			if (msg.message == WM_QUIT)
//				break;
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else {
//			// run game code    
//			frameCount++;
//			if (GetTime() > 1.0f)
//			{
//				fps = frameCount;
//				frameCount = 0;
//				StartTimer();
//			}
//
//			frameTime = GetFrameTime();
//
//			DetectInput(frameTime);
//			UpdateScene(frameTime);
//			DrawScene();
//		}
//	}
//	return msg.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hwnd,
//	UINT msg,
//	WPARAM wParam,
//	LPARAM lParam)
//{
//	switch (msg)
//	{
//	case WM_KEYDOWN:
//		if (wParam == VK_ESCAPE) {
//			DestroyWindow(hwnd);
//		}
//		return 0;
//
//	case WM_DESTROY:
//		PostQuitMessage(0);
//		return 0;
//	}
//	return DefWindowProc(hwnd,
//		msg,
//		wParam,
//		lParam);
//}