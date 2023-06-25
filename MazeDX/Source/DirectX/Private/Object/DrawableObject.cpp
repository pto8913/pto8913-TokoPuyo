
#include "Object/DrawableObject.h"

#include "Render/Factory/Bindable.h"


DrawableObject::~DrawableObject()
{
	//Util::SafeDelete(m_pIndexBuffer.get());
	//Util::SafeDelete(m_pVertexBuffer.get());
}

void DrawableObject::AddTask(std::shared_ptr<Bindable> task)
{
	tasks.push_back(task);
}
void DrawableObject::ExecuteTasks(DirectX11& dx)
{
	for (auto task : tasks)
	{
		task.get()->Bind(dx);
	}
	
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
DirectX::XMMATRIX DrawableObject::GetWorldTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(worldPosition.x, worldPosition.y, worldPosition.z);
}
