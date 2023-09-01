
#include "Component/ActorComponent.h"

#include "Object/Actor.h"

ActorComponent::ActorComponent(Actor* inOwner)
	: ComponentBase(inOwner)
{
}
ActorComponent::~ActorComponent()
{
	pOwner = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------

// -----------------------------------
// Main : Transform
// -----------------------------------
FVector ActorComponent::GetActorLocation()
{
	return mLocalLocation;
}
void ActorComponent::SetActorLocation(const FVector& in)
{
	mLocalLocation = in;
}
void ActorComponent::AddActorLocation(const FVector& in)
{
	mLocalLocation += in;
}
FRotator ActorComponent::GetActorRotation()
{
	return mLocalRotation;
}
void ActorComponent::SetActorRotation(const FRotator& in)
{
	mLocalRotation = in;
}
FVector ActorComponent::GetActorScale()
{
	return mLocalScale;
}
void ActorComponent::SetActorScale(const FVector& in)
{
	mLocalScale = in;
}
