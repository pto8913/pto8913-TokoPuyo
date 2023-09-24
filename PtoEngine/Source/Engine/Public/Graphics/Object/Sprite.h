#pragma once

#include "ObjectBase.h"
#include "Graphics/Render/TransformConstantBuffer.h"

class DirectX11;

class ScreenTexture;

class Sprite : public ObjectBase
{
public:
	Sprite() = default;
	Sprite(DirectX11& dx, const std::wstring& inFileName, std::wstring Tag, DirectX::XMFLOAT2 inSize);
	virtual ~Sprite();

	void ExecuteTasks(DirectX11& dx);
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
	std::shared_ptr<ScreenTexture> pScreenTexture = nullptr;
};