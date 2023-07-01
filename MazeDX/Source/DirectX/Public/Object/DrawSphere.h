#pragma once

#include "DrawableObject.h"

class DirectX11;

class DrawSphere : public DrawableObject
{
public:
	DrawSphere(DirectX11& dx, float radius);

	DirectX::XMMATRIX GetTransformXM() const noexcept;
};