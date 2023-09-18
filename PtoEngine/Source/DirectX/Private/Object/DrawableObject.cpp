
#include "Object/DrawableObject.h"

#include "Render/Bindable.h"
#include "Render/Topology.h"
#include "Render/IndexBuffer.h"
//#include "Render/VertexBuffer.h"

DrawableObject::~DrawableObject()
{
	auto iter = tasks.begin();
	while (iter != tasks.end())
	{
		auto& task = *iter;
		if (task != nullptr)
		{
			task.reset();
			task = nullptr;
			iter = tasks.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	tasks.clear();

	pIndexBuffer.reset();
	pIndexBuffer = nullptr;

	pTopology.reset();
	pTopology = nullptr;

	pTCB.reset();
	pTCB = nullptr;
}

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
	if (visibility)
	{
		if (pTopology)
		{
			pTopology->Bind(dx);
		}
		if (pIndexBuffer)
		{
			pIndexBuffer->Bind(dx);
		}
		//pVertexBuffer->Bind(dx);
		for (auto task : tasks)
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