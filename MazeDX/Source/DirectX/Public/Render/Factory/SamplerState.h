#pragma once

#include "Bindable.h"

class DirectX11;

/*
* The process of interpolation when scaling texture.
*/
class SamplerState : public Bindable
{
public:
	SamplerState(DirectX11& dx, UINT inSlot);
	~SamplerState();

	static std::shared_ptr<SamplerState> Make(DirectX11& dx, UINT inSlot);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(UINT inSlot)
	{
		using namespace std::string_literals;
		return typeid(SamplerState).name() + "#"s + std::to_string(inSlot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(slot);
	}
private:
	UINT slot;
	ID3D11SamplerState* m_pSamplerState;
};