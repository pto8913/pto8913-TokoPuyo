
#include "Input/Camera.h"

#include "Core/DirectX.h"

using namespace DirectX;

Camera::Camera(DirectX11& dx, std::string inName, XMFLOAT3 inDefaultPos, float inDefaultPitch, float inDefaultYaw, bool inTethered)
	:
	name(std::move(inName)),
	defaultPos(XMLoadFloat3(&inDefaultPos)),
	defaultPitch(inDefaultPitch),
	defaultYaw(inDefaultYaw),
	tethered(inTethered),
	m_CameraProjection(dx, 1.f, 9.f / 16.f, 0.5f, 400.f),
	m_CameraIndicator(dx)
{
	if (tethered)
	{
		SetLocation(defaultPos);
	}
	location = { 0.f, 3.f, 0.f };

	camView = XMMatrixLookAtLH(location, camLookAt, WorldUpVector);

	camProjection = XMMatrixOrthographicRH(200.f, 360.f, 1.f, 1000.f);

	//camProjection = XMMatrixPerspectiveFovLH(
	//	0.4f * 3.14f,
	//	800.f / 600.f,
	//	1.f,
	//	1000.f
	//);
	Reset(dx);
}

void Camera::Reset(DirectX11& dx) noexcept
{
	if (!tethered)
	{
		SetLocation(defaultPos);
	}

	SetRotation({ camPitch, camYaw, camRoll });
	m_CameraProjection.Reset(dx);
}

std::string Camera::GetName() const noexcept
{
	return name;
}
XMMATRIX Camera::GetView() const
{
	return camView;
}
XMMATRIX Camera::GetProjection() const
{
	return camProjection;
	//return m_CameraProjection.GetMatrix();
}

void Camera::SetLocation(const DirectX::XMVECTOR& inPos) noexcept
{
	location = inPos;
	m_CameraIndicator.SetLocation(location);
	m_CameraProjection.SetLocation(location);
}
void Camera::SetRotation(const DirectX::XMVECTOR& inRot) noexcept
{
	rotation = inRot;
	m_CameraIndicator.SetRotation(rotation);
	m_CameraProjection.SetRotation(rotation);
}

void Camera::AddMoveLeftRight(float Val)
{
	moveInputLeftRight = Val * moveSpeed;
}
void Camera::AddMoveBackForward(float Val)
{
	moveInputBackForward = Val * moveSpeed;
}
void Camera::AddMoveUpDownward(float Val)
{
	moveInputUpDown = Val * moveSpeed;
}
void Camera::AddYaw(float Val)
{
	camYaw += Val * rotationSpeed;

	//float newYaw = XMVectorGetY(rotation) + Val * rotationSpeed;
	//XMVectorSetY(rotation, Math::wrap_angle(newYaw));
	camYaw = Math::wrap_angle(camYaw);
}
void Camera::AddPitch(float Val)
{
	camPitch += Val * rotationSpeed;
	//float newPitch = XMVectorGetX(rotation) + Val * rotationSpeed;

	//XMVectorSetX(rotation, std::clamp(newPitch, 0.995f * -PI / 2.f, 0.995f * PI / 2.f));
	camPitch = std::clamp(camPitch, 0.995f * -PI / 2.f, 0.995f * PI / 2.f);
}

XMVECTOR Camera::GetForwardVector()
{
	//return XMVector3Normalize(
	//	{ 
	//		cos(rotation.x) * sin(rotation.y),
	//		-sin(rotation.x),
	//		cos(rotation.x) * cos(rotation.y)
	//	}
	//);

	//return XMVector3TransformCoord(
	//	WorldForwardVector,
	//	XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.f)
	//);

	return camForward;
}
XMVECTOR Camera::GetRightVector()
{
	//return XMVector3NormalizeEst(
	//	{
	//		cos(rotation.y), 0, -sin(rotation.y)
	//	}
	//);

	//return XMVector3Cross(GetUpVector(), GetForwardVector());

	return camRight;
}
XMVECTOR Camera::GetUpVector()
{
	//return XMVector3TransformCoord(
	//	WorldUpVector,
	//	XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, 0.f)
	//);

	//return XMVector3Cross(GetRightVector(), GetForwardVector());

	return camUp;
}

void Camera::ExecuteTasks(DirectX11& dx)
{
	location += camForward * moveInputBackForward + camRight * moveInputLeftRight + camUp * moveInputUpDown;

	XMMATRIX MRotation = XMMatrixRotationRollPitchYaw(camPitch, camYaw, camRoll);

	camLookAt = XMVector3TransformCoord(WorldForwardVector, MRotation);
	camUp = XMVector3TransformCoord(WorldUpVector, MRotation);

	camForward = XMPlaneNormalize(camLookAt);
	camRight = XMVector3Cross(camUp, camLookAt);
	camRight = XMPlaneNormalize(camRight);

	camLookAt = location + camLookAt;

	camView = XMMatrixLookAtLH(location, camLookAt, WorldUpVector);

	moveInputBackForward = 0.f;
	moveInputLeftRight = 0.f;
	moveInputUpDown = 0.f;

	dx.SetCameraView(camView);
	dx.SetCameraProjection(camProjection);
	dx.SetCameraLocation(location);
	dx.SetCameraRotation(rotation);

	OnCameraViewChanged.Broadcast(camView);
	OnCameraProjectionChanged.Broadcast(camProjection);
	OnCameraLocationChanged.Broadcast(location);
	OnCameraRotationChanged.Broadcast(rotation);
}