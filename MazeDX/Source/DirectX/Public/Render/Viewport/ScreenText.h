#pragma once

#include "Object/DrawableObject.h"

#include "Render/Factory/TransformConstantBuffer.h"

class DirectX11;

class TransformConstantBuffer;
class Texture2D;

class ScreenText : public DrawableObject
{
public:
	ScreenText(DirectX11& dx, UINT inWidth, UINT inHeight);

	void UpdateText(std::wstring inText);
	virtual void ExecuteTasks(DirectX11& dx) override;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept override;

protected:
	std::wstring text;

	std::shared_ptr<TransformConstantBuffer> m_pTCB;
	std::shared_ptr<Texture2D> m_pTexture2D;

	const TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};
};