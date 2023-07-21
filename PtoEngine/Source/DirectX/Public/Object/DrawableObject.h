#pragma once

#include "Object/ObjectBase.h"

class IndexBuffer;
class ConstantBufferEx;

class Topology;

class Bindable;
class DirectX11;

class DrawableObject : public ObjectBase
{
public:
	DrawableObject() = default;

	void AddTask(std::shared_ptr<Bindable> task);
	void InitializeTasks();
	virtual void ExecuteTasks(DirectX11& dx);

	virtual void SetVisibility(bool inVisible);
	virtual bool GetVisibility();
protected:
	bool visibility = true;
	std::shared_ptr<IndexBuffer> m_pIndexBuffer;
	//std::shared_ptr<VertexBuffer<DX::FVertex>> m_pVertexBuffer;
	std::shared_ptr<Topology> m_pTopology;

	std::vector<std::shared_ptr<Bindable>> tasks;
};