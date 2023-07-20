
#include "Core/DirectX.h"

#include "Render/Factory/DepthStencilView.h"
#include "Render/Factory/RenderTargetView.h"

// ------------------------------------------------------------------------------------------------------------
// Main
// ------------------------------------------------------------------------------------------------------------
DirectX11::DirectX11(HINSTANCE hInstance, HWND hWnd, UINT width, UINT height)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	// ------------------------------------------------------
	// Create Device3D
	// ------------------------------------------------------
	//Describe our SwapChain Buffer
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = width;
	bufferDesc.Height                  = height;
	bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.RefreshRate.Numerator   = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc         = bufferDesc;
	swapChainDesc.SampleDesc.Count   = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount        = 1;
	swapChainDesc.OutputWindow       = m_hWnd;
	swapChainDesc.Windowed           = true;
	swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
	//swapChainDesc.Flags              = 0;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_11_1,
	};
	
	HRESULT result = D3D11CreateDeviceAndSwapChain(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		(UINT)ARRAYSIZE(featureLevels),
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

	m_pRenderTargetView = std::make_shared<RenderTargetView>(*this, pBackBuffer.Get());
	m_pDepthStencilView = std::make_shared<DepthStencilView>(*this, width, height);

	bInitialized = true;
}
DirectX11::~DirectX11()
{
	m_pSwapChain->SetFullscreenState(false, NULL);
	PostMessage(m_hWnd, WM_DESTROY, 0, 0);

	Util::SafeRelease(m_pSwapChain);
	Util::SafeRelease(m_pID3DDevice);
	Util::SafeRelease(m_pID3DContext);

	m_pRenderTargetView.reset();
	m_pDepthStencilView.reset();
}

// ------------------------------------------------------
// Main : Frame
// ------------------------------------------------------
void DirectX11::BeginFrame()
{
	// TODO: Add rendering logic here.
	m_pRenderTargetView->Clear(*this);
	m_pDepthStencilView->Clear(*this);

	m_pID3DContext->OMSetRenderTargets(1, &m_pRenderTargetView->Get(), m_pDepthStencilView->Get());
	m_pID3DContext->OMSetBlendState(0, 0, 0xffffffff);

	//Set the default blend state (no blending) for opaque objects
}
HRESULT DirectX11::EndFrame()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	return m_pSwapChain->Present(0, 0);
}

void DirectX11::DrawIndexed(UINT count)
{
	m_pID3DContext->DrawIndexed(count, 0, 0);
}

// ------------------------------------------------------
// Main : Camera
// ------------------------------------------------------
void DirectX11::SetCameraView(const DirectX::XMMATRIX& InCameraView)
{
	CameraView = InCameraView;
}
DirectX::XMMATRIX DirectX11::GetCameraView() const
{
	return CameraView;
}
void DirectX11::SetCameraProjection(const DirectX::XMMATRIX& InCameraProjection)
{
	CameraProjection = InCameraProjection;
}
DirectX::XMMATRIX DirectX11::GetCameraProjection() const
{
	return CameraProjection;
}
void DirectX11::SetCameraLocation(const DirectX::XMVECTOR& InCameraLocation)
{
	CameraLocation = InCameraLocation;
}
DirectX::XMVECTOR DirectX11::GetCameraLocation() const
{
	return CameraLocation;
}
void DirectX11::SetCameraRotation(const DirectX::XMVECTOR& InCameraRotation)
{
	CameraRotation = InCameraRotation;
}
DirectX::XMVECTOR DirectX11::GetCameraRotation() const
{
	return CameraRotation;
}