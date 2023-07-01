#pragma once


#include "Object/ObjectBase.h"

class IndexBuffer;
class VertexBuffer;
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

protected:
	std::shared_ptr<IndexBuffer> m_pIndexBuffer;
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;

	std::vector<std::shared_ptr<Bindable>> tasks;
};