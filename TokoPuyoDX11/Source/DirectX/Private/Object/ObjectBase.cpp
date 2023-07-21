
#include "Object/ObjectBase.h"

#include "Core/DirectX.h"

using namespace DirectX;

// ----------------------------------------------------
// Transform
// ----------------------------------------------------
XMVECTOR ObjectBase::GetLocation() const noexcept
{
	return location;
}
void ObjectBase::SetLocation(const DirectX::XMVECTOR& inPos) noexcept
{
	location = inPos;
}

XMVECTOR ObjectBase::GetRotation() const noexcept
{
	return rotation;
}
void ObjectBase::SetRotation(const DirectX::XMVECTOR& inRotation) noexcept
{
	rotation = inRotation;
}

XMVECTOR ObjectBase::GetScale() const noexcept
{
	return scale;
}
void ObjectBase::SetScale(const DirectX::XMVECTOR& inScale) noexcept
{
	scale = inScale;
}

XMMATRIX ObjectBase::GetTransformXM(DirectX11&) const noexcept
{
	return
		XMMatrixRotationRollPitchYawFromVector(rotation) *
		//XMMatrixScaling(XMLoadFloat3(&scale)) * 
		XMMatrixTranslationFromVector(location);
}