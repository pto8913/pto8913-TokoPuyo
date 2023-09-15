#pragma once

#include "Bindable.h"

class DirectX11;

class DepthStencilState : public Bindable
{
public:
	enum DSSType
	{
		None,
		SkyMap,
	};

	DepthStencilState(DirectX11& dx, DSSType inType);
	virtual ~DepthStencilState();

	static std::shared_ptr<DepthStencilState> Make(DirectX11& dx, DSSType inType);

	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(DepthStencilState::DSSType type)
	{
		return "DepthStencilState_" + std::to_string(type);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(type);
	}
protected:
	DSSType type;
	ID3D11DepthStencilState* pDepthStencilState = nullptr;
};