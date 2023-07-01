#pragma once

#include "DirectX/DirectXHead.h"

class ObjectBase
{
public:
	// --------------------------------------------------------------------------------------------------------
	// Main
	// --------------------------------------------------------------------------------------------------------
	
	// ----------------------------------------------------
	// Main: Transform
	// ----------------------------------------------------
	DirectX::XMVECTOR GetLocation() const noexcept;
	virtual void SetLocation(const DirectX::XMVECTOR& inPos) noexcept;

	// x: pitch, y: yaw, z: roll
	DirectX::XMVECTOR GetRotation() const noexcept;
	// x: pitch, y: yaw, z: roll
	virtual void SetRotation(const DirectX::XMVECTOR& inRotation) noexcept;

	DirectX::XMVECTOR GetScale() const noexcept;
	void SetScale(const DirectX::XMVECTOR& inScale) noexcept;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept;
protected:
	// --------------------------------------------------------------------------------------------------------
	// State
	// --------------------------------------------------------------------------------------------------------
	
	// ----------------------------------------------------
	// State: Transform
	// ----------------------------------------------------
	DirectX::XMVECTOR location = { 0, 0, 0 };
	// x: pitch, y: yaw, z: roll
	DirectX::XMVECTOR rotation = { 0, 0, 0 };
	DirectX::XMVECTOR scale = { 1, 1, 1 };
};