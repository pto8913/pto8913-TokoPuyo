
#include "Graphics/Object/DrawableObject3D.h"

#include "Graphics/Render/Topology.h"
#include "Graphics/Render/IndexBuffer.h"

void DrawableObject3D::ExecuteTasks(DirectX11& dx)
{
	pTopology->Bind(dx);
	pIndexBuffer->Bind(dx);
	pVertexBuffer->Bind(dx);
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
}
DrawableObject3D::~DrawableObject3D()
{
	pVertexBuffer.reset();
	pVertexBuffer = nullptr;
}
