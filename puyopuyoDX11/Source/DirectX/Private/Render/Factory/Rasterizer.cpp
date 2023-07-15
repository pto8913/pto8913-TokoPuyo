
#include "Render/Factory/Rasterizer.h"

#include "Render/Manager/BindableManager.h"

Rasterizer::Rasterizer(DirectX11& dx, RasterizerType inType, UINT inSize)
	: type(inType), size(inSize)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	switch (inType)
	{
	case Wire:
		desc.FillMode = D3D11_FILL_WIREFRAME;
		desc.CullMode = D3D11_CULL_NONE;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_1);
		break;
	case Transparent:
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = true;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_1);
		desc.FrontCounterClockwise = false;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_2);
		break;
	case Transparent1:
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = true;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_1);
		break;
	case Transparent2:
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_2);
		break;
	default:
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.FrontCounterClockwise = false;
		GetDevice(dx)->CreateRasterizerState(&desc, &m_pRasterizer_1);
		break;
	}
}
Rasterizer::~Rasterizer()
{
	//Util::SafeRelease(m_pRasterizer_1);
	//Util::SafeRelease(m_pRasterizer_2);
}

std::shared_ptr<Rasterizer> Rasterizer::Make(DirectX11& dx, RasterizerType inType, UINT inSize)
{
	return BindableManager::Make<Rasterizer>(dx, inType, inSize);
}
void Rasterizer::Bind(DirectX11&, ID3D11DeviceContext* pContext)
{
	switch (type)
	{
	case Wire:
		pContext->RSSetState(m_pRasterizer_1);
		pContext->DrawIndexed(size, 0, 0);
		break;
	case Transparent:
		pContext->RSSetState(m_pRasterizer_1);
		pContext->DrawIndexed(size, 0, 0);
		pContext->RSSetState(m_pRasterizer_2);
		pContext->DrawIndexed(size, 0, 0);
		break;
	case Transparent1:
		pContext->RSSetState(m_pRasterizer_1);
		pContext->DrawIndexed(size, 0, 0);
		break;
	case Transparent2:
		pContext->RSSetState(m_pRasterizer_2);
		pContext->DrawIndexed(size, 0, 0);
		break;
	default:
		pContext->RSSetState(m_pRasterizer_1);
		pContext->DrawIndexed(size, 0, 0);
		break;
	}
}

void Rasterizer::Get(ID3D11RasterizerState& Out1, ID3D11RasterizerState& Out2)
{
	if (&Out1 != nullptr)
	{
		Out1 = *m_pRasterizer_1;
	}
	if (&Out2 != nullptr)
	{
		Out2 = *m_pRasterizer_2;
	}
}