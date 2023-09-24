
#include "Component/MovementComponent.h"

#include "Algorithm/Math.h"

#include "Object/Actor.h"

#include "Helper/VectorHelper.h"

MovementComponent::MovementComponent(Actor* inOwner)
	: ActorComponent(inOwner)
{
}
MovementComponent::~MovementComponent()
{

}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void MovementComponent::BeginPlay(DirectX11& dx)
{
	ActorComponent::BeginPlay(dx);


}
void MovementComponent::Tick(DirectX11& dx, float deltaSec)
{
	ActorComponent::Tick(dx, deltaSec);

	const auto UnderPlayer = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * -10.f;

	
	switch (GetMovementMode())
	{
	case EMovementMode::InAir:
		InAirControl();
		break;
	case EMovementMode::Walking:
		break;
	default:
		break;
	}
}

void MovementComponent::StartInput()
{
	mInputStartTime = std::chrono::system_clock::now();
	bIsStarted = true;
}

void MovementComponent::AddVelocity(FVector worldDirection, float scale)
{
	if (bIsStarted)
	{
		auto duration = std::chrono::system_clock::now() - mInputStartTime;
		mInputDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

		auto currLocation = GetOwner()->GetActorLocation();
		mVelocity = worldDirection * scale * mAcceleration * (mInputDuration.count() / 1000.f);
		mVelocity = Vector::Clamp(mVelocity, 0, mMaxMoveSpeed);
#if _DEBUG
		OutputDebugStringA(("Clamped Velocity : " + std::to_string(mVelocity.Length()) + " = " + std::to_string(worldDirection.Length()) + " * " + std::to_string(scale) + " * " + std::to_string(mAcceleration) + " * " + std::to_string(mInputDuration.count()) + " / 1000.f\n").c_str());
#endif
		pOwner->SetActorLocation(currLocation + mVelocity);
		mLastTickLocation = currLocation;

#if _DEBUG
		//OutputDebugStringA(("Velocity2 : " + mVelocity.ToString() + "\n").c_str());
		OutputDebugStringA(("Owner Loc : " + pOwner->GetActorLocation().ToString() + "\n").c_str());
#endif
	}
}
FVector MovementComponent::GetVelocity()
{
	return mVelocity;
}
void MovementComponent::EndInput()
{
	bIsStarted = false;
	mInputDuration = mInputDuration.zero();
}

void MovementComponent::SetMaxMoveSpeed(float in)
{
	mMaxMoveSpeed = in;
}

bool MovementComponent::IsStarted() const noexcept
{
	return bIsStarted;
}

void MovementComponent::StopMovementImmediately()
{
	mVelocity = FVector::Zero;
}

// -------------------------
// Main : MovementMode
// -------------------------
EMovementMode MovementComponent::GetMovementMode()
{
	return mMovementMode;
}
void MovementComponent::SetMovementMode(const EMovementMode& in)
{
	mMovementMode = in;
	OnMovementModeChanged(in);
}
void MovementComponent::OnMovementModeChanged(const EMovementMode& in)
{
	switch (in)
	{
	case EMovementMode::InAir:
		mInAirStartTime = std::chrono::system_clock::now();
		break;
	case EMovementMode::Walking:
		break;
	default:
		break;
	}
}

// -------------------------
// Main : InAir
// -------------------------
void MovementComponent::InAirControl()
{
	auto duration = std::chrono::system_clock::now() - mInAirStartTime;
	mInAirDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	if (bEnableGravity)
	{
		mVelocity.z += mGravityScale * (mInAirDuration.count() / 1000.f);
	}
}