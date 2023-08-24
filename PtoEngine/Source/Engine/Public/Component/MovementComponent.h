#pragma once

#include "Component/ActorComponent.h"

#include "Engine/Vector.h"

#include <chrono>

enum class EMovementMode
{
	Walking,
	Flying,

};

class MovementComponent : public ActorComponent
{
public:
	MovementComponent(std::shared_ptr<Actor> inOwner);
	virtual ~MovementComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void StartInput();
	void AddVelocity(FVector worldDirection, float scale = 1.f);
	FVector GetVelocity();
	void EndInput();

	void SetMaxMoveSpeed(float in);

	// ------------------------------------------------------
	// Settings
	// ------------------------------------------------------
	/* mMaxMoveSpeed meter per 1 minute. */
	float mMaxMoveSpeed = 300.f;
	float mAcceleration = 50.f;
	float mJumpPower = 500.f;
	float mGravityScale = 9.8f;

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

	// ---------------------------
	// State
	// ---------------------------
	std::chrono::system_clock::time_point mInputStartTime;
	std::chrono::milliseconds mInputDuration;

};