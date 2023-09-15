#pragma once

#include "Component/ComponentBase.h"
#include "Math/Rotator.h"
#include "Math/Vector.h"

class Actor;

class ActorComponent : public ComponentBase
{
public:
	ActorComponent(Actor* inOwner);
	virtual ~ActorComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(ActorComponent).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual FVector GetActorLocation();
	virtual void SetActorLocation(const FVector& in);
	virtual void AddActorLocation(const FVector& in);
	virtual FRotator GetActorRotation();
	virtual void SetActorRotation(const FRotator& in);
	virtual FVector GetActorScale();
	virtual void SetActorScale(const FVector& in);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	FVector mLocalLocation;
	FRotator mLocalRotation;
	FVector mLocalScale;
};