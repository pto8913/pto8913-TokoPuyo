
#include "Render/Factory/DepthStencilView.h"

DepthStencilView::DepthStencilView(DirectX11& dx, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width              = width;
	textureDesc.Height             = height;
	textureDesc.ArraySize          = 1;
	textureDesc.MipLevels          = 1;
	textureDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.Usage              = D3D11_USAGE_DEFAULT;

	HRESULT result = GetDevice(dx)->CreateTexture2D(
		&textureDesc,
		NULL,
		&m_pDepthStencilViewBuffer
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create CreateTexture2D.", L"Create CreateTexture2D Error", MB_OK);
		assert(false);
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format                  = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	viewDesc.Flags                   = 0;
	viewDesc.ViewDimension           = D3D11_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2DArray.MipSlice = 0;

	result = GetDevice(dx)->CreateDepthStencilView(
		m_pDepthStencilViewBuffer,
		NULL,
		&m_pDepthStencilView
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create DepthStencilView.", L"Create DepthStencilView Error", MB_OK);
		assert(false);
	}
}
DepthStencilView::DepthStencilView(DirectX11& dx, ID3D11Texture2D* pTexture, UINT face)
{
	D3D11_TEXTURE2D_DESC textureDesc = {};
	pTexture->GetDesc(&textureDesc);

	D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	viewDesc.Flags = 0;
	viewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MipSlice = 0;
	viewDesc.Texture2DArray.ArraySize = 1;
	viewDesc.Texture2DArray.FirstArraySlice = face;

	HRESULT result = GetDevice(dx)->CreateDepthStencilView(
		pTexture, 
		&viewDesc, 
		&m_pDepthStencilView
	);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Can not create DepthStencilView.", L"Create DepthStencilView Error", MB_OK);
		assert(false);
	}
}
DepthStencilView::~DepthStencilView()
{
	Util::SafeRelease(m_pDepthStencilView);
	Util::SafeRelease(m_pDepthStencilViewBuffer);
}

ID3D11DepthStencilView*& DepthStencilView::Get()
{
	return m_pDepthStencilView;
}
//void DepthStencilView::Bind(DirectX11& dx, ID3D11DeviceContext* pContext)
//{
//	pContext->OMSetRenderTargets(0, nullptr, m_pDepthStencilView);
//}
void DepthStencilView::Clear(ID3D11DeviceContext* pContext)
{
	pContext->ClearDepthStencilView(
		m_pDepthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 
		1.0f, 
		0
	);
}