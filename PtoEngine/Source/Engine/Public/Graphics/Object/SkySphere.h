#pragma once

#include "Graphics/Object/DrawableObject3D.h"
#include "Graphics/Geometry/TriangleList.h"

class DirectX11;

class SkySphere : public DrawableObject3D
{
public:
	SkySphere(DirectX11& dx, float radius);

	DirectX::XMMATRIX GetTransformXM(DirectX11& dx) const noexcept override;

	virtual void ExecuteTasks(DirectX11& dx) override;
protected:
	TriangleList CreateSphere(UINT LatLines, UINT LongLines);
};