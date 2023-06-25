#pragma once

#include "DirectX/DirectXHead.h"

class Camera
{
public:
	Camera(float width, float height);

	void Update();

	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProjection() const;
	DirectX::XMVECTOR GetPosition() const;

	float GetMoveLeftRight() const;
	float GetMoveBackForward() const;
	
	void AddMoveLeftRight(float Val);
	void AddMoveBackForward(float Val);

	void AddYaw(float Val);
	void AddPitch(float Val);
protected:
	float camYaw = 0.f;
	float camPitch = 0.f;

	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;
	DirectX::XMVECTOR camPosition;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;

	DirectX::XMMATRIX camRotationMatrix = DirectX::XMMATRIX();

	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR DefaultRightward = DirectX::XMVectorSet(1, 0, 0, 0);

	DirectX::XMVECTOR camForward = DirectX::XMVectorSet(0, 0, 1, 0);
	DirectX::XMVECTOR camRightward = DirectX::XMVectorSet(1, 0, 0, 0);

	float moveLeftRight = 0.f;
	float moveBackForward = 0.f;
};