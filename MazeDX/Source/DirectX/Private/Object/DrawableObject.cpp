
#include "Object/DrawableObject.h"

#include "Render/Factory/Bindable.h"
#include "Render/Factory/Topology.h"
#include "Render/Factory/IndexBuffer.h"
#include "Render/Factory/VertexBuffer.h"


DrawableObject::~DrawableObject()
{
	//Util::SafeDelete(m_pIndexBuffer.get());
	//Util::SafeDelete(m_pVertexBuffer.get());
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
	//m_pIndexBuffer->Bind(dx);
	//m_pVertexBuffer->Bind(dx);
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
	dx.DrawIndexed(m_pIndexBuffer->GetCount());
}
void DrawableObject::Update(DirectX11& dx)
{

}

// ----------------------------------------------------
// Transform
// ----------------------------------------------------
void DrawableObject::SetWorldLocation(DirectX::XMFLOAT3 inPos) noexcept
{
	worldPosition = inPos;
}
void DrawableObject::SetWorldRotation(float inRoll, float inPitch, float inYaw) noexcept
{
	roll = inRoll;
	pitch = inPitch;
	yaw = inYaw;
}
void DrawableObject::SetWorldScale(DirectX::XMMATRIX inScale) noexcept
{
	worldScale = inScale;
}
DirectX::XMMATRIX DrawableObject::GetWorldTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z) *
		worldScale;
}
