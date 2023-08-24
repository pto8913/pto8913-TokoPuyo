#pragma once

#include "Object/DrawableObject2D.h"

class DirectX11;

class WidgetBase : private DrawableObject2D
{
public:
	WidgetBase(DirectX11& dx, UINT windowSizeW, UINT windowSizeH);

	virtual void ExecuteTasks(DirectX11& dx) override final;

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

protected:
	virtual void Draw() = 0;
	void DrawInternal(DirectX11& dx);

	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};

	ID2D1RenderTarget* pRt2D = nullptr;
	IDXGIKeyedMutex* pMutex11 = nullptr;
	IDXGIKeyedMutex* pMutex10 = nullptr;
};