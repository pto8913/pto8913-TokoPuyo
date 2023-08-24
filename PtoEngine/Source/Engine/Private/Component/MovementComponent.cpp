
#include "Component/MovementComponent.h"

#include "Algorithm/Math2.h"

#include "Object/Actor.h"

MovementComponent::MovementComponent(std::shared_ptr<Actor> inOwner)
	: ActorComponent(inOwner)
{
	SetTickEnabled(false);
}
MovementComponent::~MovementComponent()
{

}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void MovementComponent::Tick(DirectX11& dx, float deltaSec)
{
}

void MovementComponent::StartInput()
{
	mInputStartTime = std::chrono::system_clock::now();
}

void MovementComponent::AddVelocity(FVector worldDirection, float scale)
{
	auto currLocation = GetOwner()->GetActorLocation();
	mVelocity = FVector::Zero + mAcceleration * (mInputDuration.count() / 1000.f);
	pOwner->SetActorLocation(currLocation + mVelocity);
	mLastTickLocation = currLocation;

	auto duration = std::chrono::system_clock::now() - mInputStartTime;
	mInputDuration = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	OutputDebugStringA((mVelocity.ToString() + "\n").c_str());
}
FVector MovementComponent::GetVelocity()
{
	return mVelocity;
}
void MovementComponent::EndInput()
{
	mInputDuration.zero();
}

void MovementComponent::SetMaxMoveSpeed(float in)
{
	mMaxMoveSpeed = in;
}