
#include "Render/RenderTargetView.h"

//RenderTargetView::RenderTargetView(DirectX11& dx, UINT width, UINT height)
//{
//	D3D11_TEXTURE2D_DESC textureDesc = {};
//	textureDesc.Width              = width;
//	textureDesc.Height             = height;
//	textureDesc.MipLevels          = 1;
//	textureDesc.ArraySize          = 1;
//	textureDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM;
//	textureDesc.SampleDesc.Count   = 1;
//	textureDesc.SampleDesc.Quality = 0;
//	textureDesc.Usage              = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; // never do we not want to bind offscreen RTs as inputs
//	textureDesc.CPUAccessFlags     = 0;
//	textureDesc.MiscFlags          = 0;
//
//	ID3D11Texture2D* pTexture;
//	GetDevice(dx)->CreateTexture2D(
//		&textureDesc, 
//		nullptr, 
//		&pTexture
//	);
//	
//	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
//	rtvDesc.Format = textureDesc.Format;
//	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	rtvDesc.Texture2D = D3D11_TEX2D_RTV{ 0 };
//	GetDevice(dx)->CreateRenderTargetView(
//		pTexture,
//		NULL, 
//		&m_pRenderTargetView
//	);
//}
RenderTargetView::RenderTargetView(DirectX11& dx, ID3D11Texture2D* pTexture, std::optional<UINT>)
{
	GetDevice(dx)->CreateRenderTargetView(pTexture, NULL, &m_pRenderTargetView);
}
RenderTargetView::~RenderTargetView()
{
	//Util::SafeRelease(m_pRenderTargetView);
	//Util::SafeRelease(m_pRenderTargetViewBuffer);
}

ID3D11RenderTargetView*& RenderTargetView::Get()
{
	return m_pRenderTargetView;
}
void RenderTargetView::Clear(DirectX11& dx)
{
	float bgColor[4] = { 0.1, 0.1, 0.1, 1 };

	GetContext(dx)->ClearRenderTargetView(m_pRenderTargetView, bgColor);
}
