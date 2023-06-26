#pragma once

#include "ConstantBuffer.h"

class DrawableObject;
class DirectX11;

class TransformConstantBuffer : public Bindable
{
public:
	TransformConstantBuffer(DirectX11& dx, UINT slot = 0);
	virtual void InitParentRefrence(const DrawableObject& pParent) override;
	virtual void Bind(DirectX11& dx) override;

protected:
	struct Transforms
	{
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX modelView;
		DirectX::XMMATRIX modelProjection;
	};
	void Update(DirectX11& dx, const Transforms& newTransform);
	Transforms GetTransform(DirectX11& dx);
private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> m_pVCBuffer;
	const DrawableObject* m_pParent = nullptr;
};