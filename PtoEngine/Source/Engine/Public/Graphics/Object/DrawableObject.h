#pragma once

#include "ObjectBase.h"
#include "Graphics/Render/TransformConstantBuffer.h"

class IndexBuffer;
class ConstantBufferEx;

class Topology;

class Bindable;
class DirectX11;

class DrawableObject : public ObjectBase
{
public:
	DrawableObject() = default;
	virtual ~DrawableObject();

	void AddTask(std::shared_ptr<Bindable> task);
	void InitializeTasks();
	virtual void ExecuteTasks(DirectX11& dx);

	virtual void SetVisibility(bool inVisible);
	virtual bool GetVisibility();
protected:
	bool visibility = true;
	std::shared_ptr<IndexBuffer> pIndexBuffer = nullptr;
	//std::shared_ptr<VertexBuffer<DX::FVertex>> pVertexBuffer = nullptr;
	std::shared_ptr<Topology> pTopology = nullptr;
	std::shared_ptr<TransformConstantBuffer> pTCB = nullptr;

	std::vector<std::shared_ptr<Bindable>> tasks;
};