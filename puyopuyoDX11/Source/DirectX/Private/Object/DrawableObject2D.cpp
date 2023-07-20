
#include "Object/DrawableObject2D.h"

#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"

DrawableObject2D::~DrawableObject2D()
{
	DrawableObject::~DrawableObject();
	m_pVertexBuffer.reset();
}
void DrawableObject2D::ExecuteTasks(DirectX11& dx)
{
	m_pTopology->Bind(dx);
	m_pIndexBuffer->Bind(dx);
	m_pVertexBuffer->Bind(dx);
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
}

