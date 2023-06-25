
#include "Render/Factory/Rasterizer.h"

#include "Render/Manager/BindableManager.h"

Rasterizer::Rasterizer(DirectX11& dx, bool inTwoSide)
	: bTwoSide(inTwoSide)
{
	D3D11_RASTERIZER_DESC rasterDesc = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT{});
	rasterDesc.CullMode = inTwoSide ? D3D11_CULL_NONE : D3D11_CULL_BACK;

	GetDevice(dx)->CreateRasterizerState(&rasterDesc, &m_pRasterizer);
}
Rasterizer::~Rasterizer()
{
	Util::SafeRelease(m_pRasterizer);
}

std::shared_ptr<Rasterizer> Rasterizer::Make(DirectX11& dx, bool inTwoSide)
{
	return BindableManager::Make<Rasterizer>(dx, inTwoSide);
}
void Rasterizer::Bind(DirectX11& dx)
{
	GetContext(dx)->RSSetState(m_pRasterizer);
}