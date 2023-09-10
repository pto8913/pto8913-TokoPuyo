#pragma once

#include "Object/DrawableObject2D.h"

#include "Render/TransformConstantBuffer.h"

class DirectX11;

class TransformConstantBuffer;
class ScreenText;

class DrawScreenText : public DrawableObject2D
{
public:
	DrawScreenText(DirectX11& dx, float inWidth, float inHeight);

	void UpdateText(std::wstring inText, D2D1_RECT_F inRect);

	virtual void ExecuteTasks(DirectX11& dx) override;

	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

protected:
	std::wstring text;
	D2D1_RECT_F rect;

	std::shared_ptr<TransformConstantBuffer> m_pTCB;
	std::shared_ptr<ScreenText> m_pScreenText;

	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};
};