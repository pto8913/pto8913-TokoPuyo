
#include "Input/CameraProjection.h"

#include "Core/DirectX.h"

CameraProjection::CameraProjection(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ)
	: width(inWidth), height(inHeight), nearZ(inNearZ), farZ(inFarZ),
	m_pyramid(dx, inWidth, inHeight, inNearZ, inFarZ),
	defaultWidth(inWidth), defaultHeight(inHeight), defaultNearZ(inNearZ), defaultFarZ(inFarZ)
{
}

DirectX::XMMATRIX CameraProjection::GetMatrix() const
{
	return DirectX::XMMatrixPerspectiveLH(
		width, height, nearZ, farZ
	);
}

void CameraProjection::SetLocation(DirectX::XMVECTOR location)
{
	m_pyramid.SetLocation(location);
}

void CameraProjection::SetRotation(DirectX::XMVECTOR rotation)
{
	m_pyramid.SetRotation(rotation);
}

void CameraProjection::ExecuteTasks(DirectX11& dx, ID3D11DeviceContext* pContext)
{
	m_pyramid.ExecuteTasks(dx, pContext);
}

void CameraProjection::Reset(DirectX11& dx)
{
	width = defaultWidth;
	height = defaultHeight;
	nearZ = defaultNearZ;
	farZ = defaultFarZ;
	m_pyramid.SetVertices(dx, width, height, nearZ, farZ);
}