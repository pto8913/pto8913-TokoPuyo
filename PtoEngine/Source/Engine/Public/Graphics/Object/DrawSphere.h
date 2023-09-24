#pragma once

#include "DrawableObject3D.h"

class DirectX11;

class DrawSphere : public DrawableObject3D
{
public:
	DrawSphere(DirectX11& dx, float radius);

	DirectX::XMMATRIX GetTransformXM() const noexcept;
};