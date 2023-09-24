#pragma once

#include "Graphics/Object/ObjectBase.h"

#include "CameraIndicator.h"
#include "CameraProjection.h"
#include "CameraInterface.h"

#include "Algorithm/Math.h"

class DirectX11;

class Camera : public ObjectBase, public DX::ICameraInterface
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
	virtual ~Camera();

	void Reset(DirectX11& dx) noexcept;

	std::string GetName() const noexcept;

	DirectX::XMMATRIX GetView() const;
	DirectX::XMMATRIX GetProjection() const;

	virtual void SetLocation(const DirectX::XMVECTOR& inPos) noexcept override;
	virtual void SetRotation(const DirectX::XMVECTOR& inRot) noexcept override;

	virtual void AddMoveLeftRight(float Val) override;
	virtual void AddMoveBackForward(float Val) override;
	virtual void AddMoveUpDownward(float Val) override;

	virtual void AddYaw(float Val) override;
	virtual void AddPitch(float Val) override;

	virtual DirectX::XMVECTOR GetForwardVector() override;
	virtual DirectX::XMVECTOR GetRightVector() override;
	virtual DirectX::XMVECTOR GetUpVector() override;

	void ExecuteTasks(DirectX11& dx);

protected:
	// ------------------------------------------------------------------------
	// State
	// ------------------------------------------------------------------------
	bool tethered;
	std::string name;

	DirectX::XMVECTOR defaultPos;
	float defaultYaw = 0.f;
	float defaultPitch = 0.5f;

	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;
	DirectX::XMVECTOR camLookAt;

	float camYaw = 0.f;
	float camPitch = 0.f;
	float camRoll = 0.f;

	float moveSpeed = 1.f;
	float rotationSpeed = 0.001f;

	DirectX::XMVECTOR camUp = WorldUpVector;

	DirectX::XMVECTOR camForward = WorldForwardVector;
	DirectX::XMVECTOR camRight = WorldRightVector;

	float moveInputBackForward = 0.f;
	float moveInputLeftRight = 0.f;
	float moveInputUpDown = 0.f;

	CameraProjection m_CameraProjection;
	CameraIndicator m_CameraIndicator;

	DirectX11* pDX = nullptr;
};