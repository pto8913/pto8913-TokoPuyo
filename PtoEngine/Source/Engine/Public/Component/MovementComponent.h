#pragma once

#include "Component/ActorComponent.h"

#include "Math/Vector.h"

#include <chrono>

enum class EMovementMode
{
	Walking,
	InAir,
	Flying,
};

class MovementComponent : public ActorComponent
{
public:
	MovementComponent(Actor* inOwner);
	virtual ~MovementComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(MovementComponent).name() + std::string("_") + std::to_string(mID);
	}
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void StartInput();
	void AddVelocity(FVector worldDirection, float scale = 1.f);
	FVector GetVelocity();
	void EndInput();

	void SetMaxMoveSpeed(float in);
	bool IsStarted() const noexcept;

	void StopMovementImmediately();

	// -------------------------
	// Main : MovementMode
	// -------------------------
	EMovementMode GetMovementMode();
	void SetMovementMode(const EMovementMode& in);
protected:
	void OnMovementModeChanged(const EMovementMode& in);

	// -------------------------
	// Main : InAir
	// -------------------------
	void InAirControl();
public:

	// ------------------------------------------------------
	// Settings
	// ------------------------------------------------------
	/* mMaxMoveSpeed meter per 1 minute. */
	float mMaxMoveSpeed = 0.0025f;
	float mAcceleration = 0.5f;
	float mJumpPower = 500.f;
	bool bEnableGravity = true;
	float mGravityScale = 9.8f;
	bool bIsStarted = false;

	// ---------------------------
	// Settings : Ground
	// ---------------------------
	float mWalkingBrake = 2.f;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	FVector mVelocity;
	float mActualMoveSpeed;

	FVector mLastTickLocation;
	FVector mLocation;

	EMovementMode mMovementMode = EMovementMode::Walking;

	// ---------------------------
	// State
	// ---------------------------
	std::chrono::system_clock::time_point mInputStartTime;
	std::chrono::milliseconds mInputDuration;

	// ---------------------------
	// State : Air
	// ---------------------------
	std::chrono::system_clock::time_point mInAirStartTime;
	std::chrono::milliseconds mInAirDuration;
};