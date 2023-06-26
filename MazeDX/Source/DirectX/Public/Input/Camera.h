#pragma once

#include "DirectX/DirectXHead.h"

class DirectX11;

class Camera
{
public:
	Camera(float width, float height);
	Camera(
		DirectX11& dx,
		std::string inName,
		DirectX::XMVECTOR homePos = { 0.0f,0.0f,0.0f },
		float homePitch = 0.0f,
		float homeYaw = 0.0f,
		bool tethered = false
	);

	void Reset(DirectX11& dx) noexcept;

	void Update();

	std::string GetName() const noexcept;

	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProjection() const;

	DirectX::XMVECTOR GetPosition() const;
	void SetPosition(DirectX::XMVECTOR newPos);

	float GetMoveLeftRight() const;
	float GetMoveBackForward() const;
	
	void AddMoveLeftRight(float Val);
	void AddMoveBackForward(float Val);

	void AddYaw(float Val);
	void AddPitch(float Val);
protected:
	bool tethered;
	std::string name;

	DirectX::XMVECTOR homePos;
	float homeYaw = 0.f;
	float homePitch = 0.f;
	DirectX::XMVECTOR camPosition;
	float camYaw = 0.f;
	float camPitch = 0.f;

	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;
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