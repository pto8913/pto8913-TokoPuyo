
#include "Object/DrawableObject3D.h"

#include "Render/Topology.h"
#include "Render/IndexBuffer.h"

void DrawableObject3D::ExecuteTasks(DirectX11& dx)
{
	m_pTopology->Bind(dx);
	m_pIndexBuffer->Bind(dx);
	m_pVertexBuffer->Bind(dx);
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
}

