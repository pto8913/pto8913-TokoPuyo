#pragma once

#include "DrawableObject.h"
#include "Graphics/Render/VertexBuffer.h"

class DirectX11;

class DrawableObject3D : public DrawableObject
{
public:
	DrawableObject3D() = default;
	virtual ~DrawableObject3D();

	virtual void ExecuteTasks(DirectX11& dx) override;

protected:
	std::shared_ptr<VertexBuffer<DX::FVertex>> pVertexBuffer = nullptr;
};