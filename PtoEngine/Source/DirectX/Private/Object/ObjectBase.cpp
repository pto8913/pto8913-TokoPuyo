
#include "Object/ObjectBase.h"

#include "Core/DirectX.h"

#include "Math/Math.h"

using namespace DirectX;
using namespace Math;

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

XMVECTOR ObjectBase::GetForwardVector()
{
	//return XMVector3Normalize(
	//	{ 
	//		cos(rotation.x) * sin(rotation.y),
	//		-sin(rotation.x),
	//		cos(rotation.x) * cos(rotation.y)
	//	}
	//);

	return XMVector3TransformCoord(
		WorldForwardVector,
		XMMatrixRotationRollPitchYaw(XMVectorGetX(rotation), XMVectorGetY(rotation), 0)
	);
}
XMVECTOR ObjectBase::GetRightVector()
{
	//return XMVector3NormalizeEst(
	//	{
	//		cos(rotation.y), 0, -sin(rotation.y)
	//	}
	//);

	return XMVector3Cross(GetUpVector(), GetForwardVector());
}
XMVECTOR ObjectBase::GetUpVector()
{
	return XMVector3TransformCoord(
		WorldUpVector,
		XMMatrixRotationRollPitchYaw(XMVectorGetX(rotation), XMVectorGetY(rotation), 0.f)
	);

	//return XMVector3Cross(GetRightVector(), GetForwardVector());
}