#pragma once

#include "Object/DrawableObject.h"
#include "Render/VertexBuffer.h"

class DirectX11;

class DrawableObject3D : public DrawableObject
{
public:
	DrawableObject3D() = default;

	virtual void ExecuteTasks(DirectX11& dx) override;

protected:
	std::shared_ptr<VertexBuffer<DX::FVertex>> m_pVertexBuffer;
};