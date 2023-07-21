
#include "Object/DrawableObject.h"

#include "Render/Factory/Bindable.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"
//#include "Render/Factory/VertexBuffer.h"

DrawableObject::~DrawableObject()
{
	m_pIndexBuffer.reset();
	m_pTopology.reset();
	for (auto&& task : tasks)
	{
		task.reset();
	}
}
void DrawableObject::AddTask(std::shared_ptr<Bindable> task)
{
	tasks.push_back(task);
}
void DrawableObject::InitializeTasks()
{
	for (auto&& task : tasks)
	{
		task.get()->InitParentRefrence(*this);
	}
}
void DrawableObject::ExecuteTasks(DirectX11& dx)
{
	if (visibility)
	{
		m_pTopology->Bind(dx);
		m_pIndexBuffer->Bind(dx);
		//m_pVertexBuffer->Bind(dx);
		for (auto&& task : tasks)
		{
			task.get()->Bind(dx);
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