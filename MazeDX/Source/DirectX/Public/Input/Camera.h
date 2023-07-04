#pragma once

#include "Object/ObjectBase.h"

#include "Input/CameraIndicator.h"
#include "Input/CameraProjection.h"

#include "Math/Math.h"

class DirectX11;

class Camera : public ObjectBase
{
public:
	Camera(
		DirectX11& dx,
		std::string inName,
		DirectX::XMFLOAT3 defaultPos = { 0.0f,0.0f,0.0f },
		float defaultPitch = 0.0f,
		float defaultYaw = 0.0f,
		bool tethered = false
	);

	void Reset(DirectX11& dx) noexcept;

	std::string GetName() const noexcept;

	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProjection() const;

	virtual void SetLocation(const DirectX::XMVECTOR& inPos) noexcept override;
	virtual void SetRotation(const DirectX::XMVECTOR& inRot) noexcept override;

	void AddMoveLeftRight(float Val);
	void AddMoveBackForward(float Val);
	void AddMoveUpDownward(float Val);

	void AddYaw(float Val);
	void AddPitch(float Val);

	DirectX::XMVECTOR GetForwardVector();
	DirectX::XMVECTOR GetRightVector();
	DirectX::XMVECTOR GetUpVector();

	void Update();
protected:
	bool tethered;
	std::string name;

	DirectX::XMVECTOR defaultPos;
	float defaultYaw = 0.f;
	float defaultPitch = 0.5f;

	float camYaw = 0.f;
	float camPitch = 0.f;
	float camRoll = 0.f;

	DirectX::XMMATRIX camProjection;

	float moveSpeed = 100.f;
	float rotationSpeed = 0.001f;

	DirectX::XMMATRIX camView;
	DirectX::XMVECTOR camLookAt = { 0.f };
	DirectX::XMVECTOR camUp = WorldUpVector;

	DirectX::XMVECTOR camForward = WorldForwardVector;
	DirectX::XMVECTOR camRight = WorldRightVector;

	float moveInputBackForward = 0.f;
	float moveInputLeftRight = 0.f;
	float moveInputUpDown = 0.f;

	CameraProjection m_CameraProjection;
	CameraIndicator m_CameraIndicator;
};