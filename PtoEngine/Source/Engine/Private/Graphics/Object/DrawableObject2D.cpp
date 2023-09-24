
#include "Graphics/Object/DrawableObject2D.h"

#include "Graphics/Render/Topology.h"
#include "Graphics/Render/IndexBuffer.h"

void DrawableObject2D::ExecuteTasks(DirectX11& dx)
{
	if (pTopology)
	{
		pTopology->Bind(dx);
	}
	if (pIndexBuffer)
	{
		pIndexBuffer->Bind(dx);
	}
	if (pVertexBuffer)
	{
		pVertexBuffer->Bind(dx);
	}
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
}
DrawableObject2D::~DrawableObject2D()
{
	pVertexBuffer.reset();
	pVertexBuffer = nullptr;
}