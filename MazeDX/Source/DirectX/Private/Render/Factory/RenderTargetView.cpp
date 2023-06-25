
#include "Render/Factory/RenderTargetView.h"

RenderTargetView::RenderTargetView(DirectX11& dx, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // never do we not want to bind offscreen RTs as inputs
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ID3D11Texture2D* pTexture;
	GetDevice(dx)->CreateTexture2D(
		&textureDesc, 
		nullptr, 
		&pTexture
	);
	
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	GetDevice(dx)->CreateRenderTargetView(
		pTexture,
		&rtvDesc, 
		&m_pRenderTargetView
	);

}
RenderTargetView::RenderTargetView(DirectX11& dx, ID3D11Texture2D* pTexture, std::optional<UINT> face)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	pTexture->GetDesc(&textureDesc);
	
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	if (face.has_value())
	{
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		rtvDesc.Texture2DArray.ArraySize = 1;
		rtvDesc.Texture2DArray.FirstArraySlice = *face;
		rtvDesc.Texture2DArray.MipSlice = 0;
	}
	else
	{
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
	}

	GetDevice(dx)->CreateRenderTargetView(
		pTexture, 
		&rtvDesc, 
		&m_pRenderTargetView
	);
}
RenderTargetView::~RenderTargetView()
{
	Util::SafeRelease(m_pRenderTargetView);
	Util::SafeRelease(m_pRenderTargetViewBuffer);
}

//void RenderTargetView::CreateRenderTargetView(IDXGISwapChain* pSwapChain, ID3D11Device* pID3DDevice)
//{
//	HRESULT result = pSwapChain->GetBuffer(
//		0, 
//		__uuidof(ID3D11Texture2D), 
//		(LPVOID*)&m_pRenderTargetViewBuffer
//	);
//	if (FAILED(result))
//	{
//		MessageBox(NULL, L"Can not get backbuffer", L"Failed Get Buffer From SwapChain", MB_OK);
//		assert(false);
//	}
//
//	result = pID3DDevice->CreateRenderTargetView(
//		m_pRenderTargetViewBuffer, 
//		NULL,
//		&m_pRenderTargetView
//	);
//	if (FAILED(result))
//	{
//		MessageBox(NULL, L"Can not create RenderTargetView.", L"Create RenderTargetView Error", MB_OK);
//		assert(false);
//	}
//}

ID3D11RenderTargetView*& RenderTargetView::Get()
{
	return m_pRenderTargetView;
}
void RenderTargetView::Clear(DirectX11& dx)
{
	GetContext(dx)->ClearRenderTargetView(m_pRenderTargetView, 0x000);
}
