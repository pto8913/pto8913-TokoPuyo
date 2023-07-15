
#include "Render/Factory/Topology.h"

#include "Render/Manager/BindableManager.h"

Topology::Topology(DirectX11&, D3D11_PRIMITIVE_TOPOLOGY inType)
	: type(inType)
{
}

std::shared_ptr<Topology> Topology::Make(DirectX11& dx, D3D11_PRIMITIVE_TOPOLOGY inType)
{
	return BindableManager::Make<Topology>(dx, inType);
}
void Topology::Bind(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology(type);
}