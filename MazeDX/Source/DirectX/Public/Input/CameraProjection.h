#pragma once

#include "DirectX/DirectXHead.h"
#include "Object/DrawPyramid.h"

class DirectX11;

class CameraProjection
{
public:
	CameraProjection(DirectX11& dx, float inWidth, float inHeight, float inNearZ, float inFarZ);

	DirectX::XMMATRIX GetMatrix() const;

	void SetLocation(DirectX::XMVECTOR location);
	void SetRotation(DirectX::XMVECTOR rotation);
	virtual void ExecuteTasks(DirectX11& dx);

	void Reset(DirectX11& dx);
private:
	float width;
	float height;
	float nearZ;
	float farZ;
	float defaultWidth;
	float defaultHeight;
	float defaultNearZ;
	float defaultFarZ;
	DrawPyramid m_pyramid;
};