#pragma once

#include "DirectX/DirectXHead.h"


class IndexBuffer;
class VertexBuffer;
class Topology;

class Bindable;
class DirectX11;

class DrawableObject
{
public:
	DrawableObject() = default;
	virtual ~DrawableObject();

	void AddTask(std::shared_ptr<Bindable> task);
	void InitializeTasks();
	void ExecuteTasks(DirectX11& dx);
	virtual void Update(DirectX11& dx);

	// ----------------------------------------------------
	// Transform
	// ----------------------------------------------------
	void SetWorldLocation(DirectX::XMFLOAT3 inPos) noexcept;
	void SetWorldRotation(float inRoll, float inPitch, float inYaw) noexcept;
	void SetWorldScale(DirectX::XMMATRIX inScale) noexcept;
	virtual DirectX::XMMATRIX GetWorldTransformXM() const noexcept;
protected:
	std::shared_ptr<IndexBuffer> m_pIndexBuffer;
	std::shared_ptr<VertexBuffer> m_pVertexBuffer;
	std::shared_ptr<Topology> m_pTopology;

	DirectX::XMFLOAT3 worldPosition = { 0, 0, 0 };
	DirectX::XMMATRIX worldScale;

	float roll = 0.f;
	float pitch = 0.f;
	float yaw = 0.f;



	std::vector<std::shared_ptr<Bindable>> tasks;
};