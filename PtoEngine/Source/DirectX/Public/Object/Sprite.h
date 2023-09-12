#pragma once

#include "DrawableObject2D.h"
#include "Render/TransformConstantBuffer.h"

class DirectX11;

class ScreenTexture;

class Sprite : public DrawableObject2D
{
public:
	Sprite() = default;
	Sprite(DirectX11& dx, const std::wstring& inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize);
	virtual ~Sprite();

	virtual void ExecuteTasks(DirectX11& dx) override;
	void UpdateTexture(const std::wstring& inFileName);

	// ----------------------------------------------------
	// Main: Transform
	// ----------------------------------------------------
	D2D_POINT_2F GetCenter();
	// x: pitch, y: yaw, z: roll
	virtual void SetRotation(const DirectX::XMVECTOR& inRotation) noexcept override;
	virtual void SetScale(const DirectX::XMVECTOR& inScale) noexcept override;
	
	virtual DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

protected:
	TransformConstantBuffer::Transforms tf = {
		DirectX::XMMatrixIdentity(),
		DirectX::XMMatrixIdentity()
	};
	std::shared_ptr<TransformConstantBuffer> pTCB = nullptr;
	std::shared_ptr<ScreenTexture> pScreenTexture = nullptr;
};