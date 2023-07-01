#pragma once

#include "ConstantBuffer.h"

class DrawableObject;
class DirectX11;


class TransformConstantBuffer : public Bindable
{
public:
	struct Transforms
	{
		DirectX::XMMATRIX WorldViewProjection;
		DirectX::XMMATRIX World;
	};
	TransformConstantBuffer(DirectX11& dx, UINT inSlot = 0);
	static std::shared_ptr<TransformConstantBuffer> Make(DirectX11& dx, UINT inSlot = 0);

	virtual void InitParentRefrence(const DrawableObject& pParent) override;
	virtual void Bind(DirectX11& dx) override;

	static std::string GenerateID(UINT inSlot)
	{
		using namespace std::string_literals;
		return typeid(TransformConstantBuffer).name() + "#"s + std::to_string(inSlot);
	}
	std::string GetID() const noexcept
	{
		return GenerateID(slot);
	}
protected:

	Transforms GetTransform(DirectX11& dx);
private:
	UINT slot;
	static std::unique_ptr<VertexConstantBuffer<Transforms>> m_pVCBuffer;
	const DrawableObject* m_pParent = nullptr;
};