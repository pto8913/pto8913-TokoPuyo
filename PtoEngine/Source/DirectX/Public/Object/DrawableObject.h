#pragma once

#include "Object/ObjectBase.h"
#include "Render/Factory/TransformConstantBuffer.h"

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
	std::shared_ptr<IndexBuffer> m_pIndexBuffer = nullptr;
	//std::shared_ptr<VertexBuffer<DX::FVertex>> m_pVertexBuffer = nullptr;
	std::shared_ptr<Topology> m_pTopology = nullptr;
	std::shared_ptr<TransformConstantBuffer> m_pTCB = nullptr;

	std::vector<std::shared_ptr<Bindable>> tasks;
};