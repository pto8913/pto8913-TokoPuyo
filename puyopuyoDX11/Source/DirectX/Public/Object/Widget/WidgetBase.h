#pragma once

#include "Object/DrawableObject2D.h"
#include "Render/Factory/TransformConstantBuffer.h"

class DirectX11;

class ScreenTextOnlyOutput;

class WidgetBase : private DrawableObject2D
{
public:
	WidgetBase(DirectX11& dx);

	virtual void ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext) override final;

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override final;

	virtual void SetVisibility(bool inVisible) override;
	virtual bool GetVisibility() override;
protected:
	virtual void Draw() = 0;
	void DrawInternal();
private:
	std::shared_ptr<TransformConstantBuffer> m_pTCB = nullptr;
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