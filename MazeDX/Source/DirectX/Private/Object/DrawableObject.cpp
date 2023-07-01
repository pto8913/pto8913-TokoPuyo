
#include "Object/DrawableObject.h"

#include "Render/Factory/Bindable.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"

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
void DrawableObject::ExecuteTasks(DirectX11& dx)
{
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
}

