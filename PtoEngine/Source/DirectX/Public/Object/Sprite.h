#pragma once

#include "DrawableObject2D.h"
#include "Render/Factory/TransformConstantBuffer.h"

class DirectX11;

class ScreenTexture;

class Sprite : public DrawableObject2D
{
public:
	Sprite() = default;
	Sprite(DirectX11& dx, const std::wstring& inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize);

	virtual void ExecuteTasks(DirectX11& dx) override;
	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;
	virtual void SetScale(const DirectX::XMVECTOR& inScale) noexcept override;

	void UpdateTexture(const std::wstring& inFileName);
protected:
	TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};
	std::shared_ptr<TransformConstantBuffer> m_pTCB;
	std::shared_ptr<ScreenTexture> m_pScreenTexture;
};