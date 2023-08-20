#pragma once

#include "Object/DrawableObject2D.h"

class DirectX11;

class ScreenTextOnlyOutput;

class WidgetBase : private DrawableObject2D
{
public:
	WidgetBase(DirectX11& dx, UINT windowSizeW, UINT windowSizeH);

	virtual void ExecuteTasks(DirectX11& dx) override final;

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

protected:
	virtual void Draw() = 0;
	void DrawInternal();
private:
	std::shared_ptr<ScreenTextOnlyOutput> m_pScreenTextOnlyOutput = nullptr;

protected:
	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};

	ID2D1RenderTarget* m_pRt2D = nullptr;
	IDXGIKeyedMutex* m_pMutex11 = nullptr;
	IDXGIKeyedMutex* m_pMutex10 = nullptr;
	D2D1_COLOR_F m_ClearColor;
};