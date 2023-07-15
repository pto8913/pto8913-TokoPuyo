
#include "Object/DrawableObject.h"

#include "Render/Factory/Bindable.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"
//#include "Render/Factory/VertexBuffer.h"

void DrawableObject::AddTask(std::shared_ptr<Bindable> task)
{
	tasks.push_back(task);
}
void DrawableObject::InitializeTasks()
{
	for (auto task : tasks)
	{
		task.get()->InitParentRefrence(*this);
	}
}
void DrawableObject::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	if (visibility)
	{
		m_pTopology->Bind(dx, pContext);
		m_pIndexBuffer->Bind(dx, pContext);
		//m_pVertexBuffer->Bind(dx, pContext);
		for (auto task : tasks)
		{
			task.get()->Bind(dx, pContext);
		}
	}
}

void DrawableObject::SetVisibility(bool visible)
{
	visibility = visible;
}
bool DrawableObject::GetVisibility()
{
	return visibility;
}