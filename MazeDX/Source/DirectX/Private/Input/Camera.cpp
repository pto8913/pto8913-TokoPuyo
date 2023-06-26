
#include "Input/Camera.h"

using namespace DirectX;

Camera::Camera(float width, float height)
{
	camPosition = XMVectorSet(0.f, 3.f, -8.f, 0.f);
	camTarget = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	camUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	camProjection = XMMatrixPerspectiveFovLH(
		0.4f * 3.14f, 
		width / height, 
		1.f, 
		1000.f
	);
}
Camera::Camera(DirectX11& dx, std::string inName, DirectX::XMVECTOR homePos, float homePitch, float homeYaw, bool tethered)
	: 
	name(std::move(inName)), 
	homePos(homePos), 
	homePitch(homePitch), 
	homeYaw(homeYaw), 
	tethered(tethered)
{
	if (tethered)
	{
		camPosition = homePos;
	}
	camPosition = XMVectorSet(0.f, 3.f, -8.f, 0.f);
	camTarget = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	camUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	camProjection = XMMatrixPerspectiveFovLH(
		0.4f * 3.14f,
		width / height,
		1.f,
		1000.f
	);
}

void Camera::Reset(DirectX11& dx) noexcept
{
	if (!tethered)
	{
		camPosition = homePos;

	}
	camPitch = homePitch;
	camYaw = homeYaw;


}

void Camera::Update()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix = XMMatrixRotationY(camYaw);
	camRightward = XMVector3TransformCoord(DefaultRightward, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	camPosition += moveLeftRight * camRightward;
	camPosition += moveBackForward * camForward;

	moveLeftRight = 0.f;
	moveBackForward = 0.f;

	camTarget = camPosition + camTarget;
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

std::string Camera::GetName() const noexcept
{
	return name;
}
DirectX::XMMATRIX Camera::GetView() const
{
	return camView;
}
DirectX::XMMATRIX Camera::GetProjection() const
{
	return camProjection;
}
DirectX::XMVECTOR Camera::GetPosition() const
{
	return camPosition;
}
void Camera::SetPosition(DirectX::XMVECTOR newPos)
{
	camPosition = newPos;
}

float Camera::GetMoveLeftRight() const
{
	return moveLeftRight;
}
float Camera::GetMoveBackForward() const
{
	return moveBackForward;
}

void Camera::AddMoveLeftRight(float Val)
{
	moveLeftRight += Val;
}
void Camera::AddMoveBackForward(float Val)
{
	moveBackForward += Val;
}

void Camera::AddYaw(float Val)
{
	camYaw += Val;
}
void Camera::AddPitch(float Val)
{
	camPitch += Val;
}
