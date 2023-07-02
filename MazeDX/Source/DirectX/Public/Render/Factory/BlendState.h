#pragma once

#include "Bindable.h"

class DirectX11;

/*
* For the rendering transparent primitives.
*/
class BlendState : public Bindable
{
public:
	BlendState(DirectX11& dx, UINT inSlot = 0);
	~BlendState();

	static std::shared_ptr<BlendState> Make(DirectX11& dx, UINT inSlot = 0);
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(UINT inSlot)
	{
		using namespace std::string_literals;
		return typeid(BlendState).name() + "#"s + std::to_string(inSlot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(slot);
	}
private:
	UINT slot;
	ID3D11BlendState* m_pBlendState;
};