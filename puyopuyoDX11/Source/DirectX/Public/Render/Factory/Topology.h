#pragma once

#include "Bindable.h"

class DirectX11;

class Topology : public Bindable
{
public:
	Topology(DirectX11& dx, D3D11_PRIMITIVE_TOPOLOGY inType);

	static std::shared_ptr<Topology> Make(
		DirectX11& dx, 
		D3D11_PRIMITIVE_TOPOLOGY inType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	virtual void Bind(DirectX11& dx, ID3D11DeviceContext* pContext) override;

	static std::string GenerateID(D3D11_PRIMITIVE_TOPOLOGY inType)
	{
		using namespace std::string_literals;
		return typeid(Topology).name() + "#"s + std::to_string(inType);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(type);
	}
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};