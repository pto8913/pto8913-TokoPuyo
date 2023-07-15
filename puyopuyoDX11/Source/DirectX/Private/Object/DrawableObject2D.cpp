
#include "Object/DrawableObject2D.h"

#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"

void DrawableObject2D::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	m_pTopology->Bind(dx, pContext);
	m_pIndexBuffer->Bind(dx, pContext);
	m_pVertexBuffer->Bind(dx, pContext);
	for (auto task : tasks)
	{
		task.get()->Bind(dx, pContext);
	}
}

