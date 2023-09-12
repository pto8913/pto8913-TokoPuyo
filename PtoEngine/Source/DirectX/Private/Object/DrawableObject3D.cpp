
#include "Object/DrawableObject3D.h"

#include "Render/Topology.h"
#include "Render/IndexBuffer.h"

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
