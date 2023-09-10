
#include "Render/DepthStencilState.h"

#include "Core/DirectX.h"
#include "Render/BindableManager.h"

DepthStencilState::DepthStencilState(DirectX11& dx, DepthStencilState::DSSType inType)
	: type(inType)
{
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	switch (type)
	{
	case SkyMap:
		dssDesc.DepthEnable = true;
		dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	default:
		break;
	}
	GetDevice(dx)->CreateDepthStencilState(&dssDesc, &m_pDepthStencilState);
}

std::shared_ptr<DepthStencilState> DepthStencilState::Make(DirectX11& dx, DepthStencilState::DSSType inType)
{
	return BindableManager::Make<DepthStencilState>(dx, inType);
}
void DepthStencilState::Bind(DirectX11& dx)
{
	GetContext(dx)->OMSetDepthStencilState(m_pDepthStencilState, 0);
}