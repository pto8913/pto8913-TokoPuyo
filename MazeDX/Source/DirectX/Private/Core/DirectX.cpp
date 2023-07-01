
#include "Core/DirectX.h"

#include "Math/Vertex.h"

#include "Input/DirectXInput.h"
#include "Input/Camera.h"

#include "Render/Factory/PixelShader.h"
#include "Render/Factory/VertexShader.h"

#include "Render/Factory/BlendState.h"

#include "Render/Factory/DepthStencilView.h"
#include "Render/Factory/RenderTargetView.h"

#include "Render/Factory/ConstantBuffer.h"

#include "Render/Viewport/ViewPort.h"

#include "Render/World/SkySphere.h"

#include "Object/DrawPlane.h"
#include "Object/DrawSphere.h"

#include "Math/Math.h"

#include <vector>

const std::wstring SHADERPATH = L"Shader/Shader.hlsl";

using namespace DirectX;

DirectX11::DirectX11(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	bInitialized = true;

	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_outputWidth = (int)max(width, 1);
	m_outputHeight = (int)max(height, 1);

	CreateResources();

	m_pDirectXInput = new DirectXInput(m_hInstance, m_hWnd);
	m_pCamera = new Camera(*this, "A", DirectX::XMFLOAT3(0, 0, 5.f), 9.f, PI / 2.f);

	m_pDrawPlane = new DrawPlane(*this, 100.f);
	m_pDrawPlane->SetLocation({ 0.f, -50.f, 50.f });

	m_pDrawSphere = new DrawSphere(*this, 25.f);
	m_pDrawSphere->SetLocation({ 0.f, 50.f, 0.f });

	/* Constant Buffer Per Frame */
	{
		light.direction = XMFLOAT3(0, -1, 0);
		light.ambient = XMFLOAT4(0.2, 0.2, 0.2, 1);
		light.diffuse = XMFLOAT4(1, 1, 1, 1);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(cbPerFrame);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		//std::string str = std::string("ConstantBufferEx Size :") + std::to_string(desc.ByteWidth);
		//MessageBox(NULL, Util::s2WString(str).c_str(), L"ConstantBufferEx Error", MB_OK);

		HRESULT result = m_pID3DDevice->CreateBuffer(
			&desc,
			NULL,
			&m_pConstantBufferEx
		);

		if (FAILED(result))
		{
			MessageBox(NULL, L"Can not create ConstantBufferEx", L"Failed ConstantBufferEx", MB_OK);
			assert(false);
		}
	}

	/* Shader */
	{
		vertexShader = VertexShader::Make(*this, SHADERPATH, "VS");
		pixelShader = PixelShader::Make(*this, SHADERPATH, "PS");
		lightShader = PixelShader::Make(*this, SHADERPATH, "D2D_PS");
		vertexShader->Bind(*this);
		pixelShader->Bind(*this);
		lightShader->Bind(*this);
	}

	/* Viewport */
	{
		m_pViewPort = new ViewPort(*this, (float)m_outputWidth, (float)m_outputHeight);
		m_pViewPort->Bind(*this);
	}
}
DirectX11::~DirectX11()
{
	CleanUp();
}

HINSTANCE const& DirectX11::GetHInstance() const
{
	return m_hInstance;
}
HWND const& DirectX11::GetHWnd() const
{
	return m_hWnd;
}

void DirectX11::CleanUp()
{
	m_pSwapChain->SetFullscreenState(false, NULL);
	PostMessage(m_hWnd, WM_DESTROY, 0, 0);

	Util::SafeDelete(m_pDirectXInput);
	Util::SafeDelete(m_pCamera);
	Util::SafeDelete(m_pViewPort);

	Util::SafeRelease(m_pSwapChain);
	Util::SafeRelease(m_pID3DDevice);
	Util::SafeRelease(m_pID3DContext);
}

// Executes the basic game loop.
void DirectX11::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		}
	);
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() != 0)
	{
		// TODO: Add your rendering code here.

		Present();
	}
}

void DirectX11::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;

	m_pDirectXInput->Update(m_hWnd, elapsedTime, m_pCamera);
	m_pCamera->Update();

	//m_pSkySphere->Update(m_pCamera);
}

void DirectX11::Present()
{
	// TODO: Add rendering logic here.

	m_pRenderTargetView->Clear(*this);
	m_pDepthStencilView->Clear(*this);

	/* Light */
	{
		constbPerFrame.light = light;
		m_pID3DContext->UpdateSubresource(m_pConstantBufferEx, 0, NULL, &constbPerFrame, 0, 0);
		m_pID3DContext->PSSetConstantBuffers(0, 1, &m_pConstantBufferEx);
	}

	m_pID3DContext->OMSetRenderTargets(1, &m_pRenderTargetView->Get(), m_pDepthStencilView->Get());

	//Set the default blend state (no blending) for opaque objects
	//m_pID3DContext->OMSetBlendState(m_pBlendState, 0, 0xffffffff);

	m_pDrawPlane->ExecuteTasks(*this);
	m_pDrawSphere->ExecuteTasks(*this);

	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT result = m_pSwapChain->Present(1, 0);
	
	//m_pViewPort->CreateViewPort(m_pID3DContext, (float)m_outputWidth, (float)m_outputHeight);

	// If the device was reset we must completely reinitialize the renderer.
	if (result == DXGI_ERROR_DEVICE_REMOVED || result == DXGI_ERROR_DEVICE_RESET)
	{
		OnDeviceLost();
	}
	else
	{
		if (FAILED(result))
		{
			assert(false);
		}
	}
}

void DirectX11::DrawIndexed(UINT count)
{
	m_pID3DContext->DrawIndexed(count, 0, 0);
}

DirectX::XMMATRIX DirectX11::GetCameraView() const
{
	return m_pCamera->GetView();
}
DirectX::XMMATRIX DirectX11::GetCameraProjection() const
{
	return m_pCamera->GetProjection();
}

// Message handlers
void DirectX11::OnActivated()
{
	// TODO: DirectX11 is becoming active window.
}

void DirectX11::OnDeactivated()
{
	// TODO: DirectX11 is becoming background window.
}

void DirectX11::OnSuspending()
{
	// TODO: DirectX11 is being power-suspended (or minimized).
}

void DirectX11::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: DirectX11 is being power-resumed (or returning from minimize).
}

void DirectX11::OnDeviceLost()
{
	// TODO: Add Direct3D resource cleanup here.
	CleanUp();
	CreateResources();

	MessageBox(NULL, L"DeviceLost", L"Create DeviceLost Error", MB_OK);
}

// ------------------------------------------------------
// Window
// ------------------------------------------------------
void DirectX11::GetDefaultSize(int& width, int& height) const noexcept
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}
void DirectX11::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);
}

// ------------------------------------------------------
// Resource
// ------------------------------------------------------
// Allocate all memory resources that change on a window SizeChanged event.
void DirectX11::CreateResources()
{
	//Describe our SwapChain Buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = m_outputWidth;
	bufferDesc.Height = m_outputHeight;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = m_hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};
	UINT totalFeatureLevels = ARRAYSIZE(featureLevels);

	UINT creationFlags = 0;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		creationFlags,
		featureLevels,
		totalFeatureLevels,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pID3DDevice,
		nullptr,
		&m_pID3DContext
	);

	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not find Driver Type.", L"Create Swap Chain Error", MB_OK);
		assert(false);
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		&pBackBuffer
	);

	m_pRenderTargetView = std::shared_ptr<RenderTargetView>{ new RenderTargetView(*this, pBackBuffer.Get()) };
	m_pDepthStencilView = std::shared_ptr<DepthStencilView>{ new DepthStencilView(*this, m_outputWidth, m_outputHeight) };

	//m_pID3DContext->OMSetRenderTargets(1, &m_pRenderTargetView->Get(), m_pDepthStencilView->Get());

	// TODO: Initialize windows-size dependent objects here.
}