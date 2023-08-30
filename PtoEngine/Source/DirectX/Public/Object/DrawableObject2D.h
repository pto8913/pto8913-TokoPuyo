#pragma once

#include "Object/DrawableObject.h"
#include "Render/Factory/VertexBuffer.h"

class DirectX11;

class DrawableObject2D : public DrawableObject
{
public:
	DrawableObject2D() = default;

	virtual void ExecuteTasks(DirectX11& dx) override;

protected:
	std::shared_ptr<VertexBuffer<DX::FVertex2D>> m_pVertexBuffer;
};