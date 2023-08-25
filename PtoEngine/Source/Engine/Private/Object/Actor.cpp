
#include "Object/Actor.h"

#include "Level/Level.h"

#include "Engine/World.h"

#include "Component/ActorComponent.h"

#include "Math/Math.h"

#include "Helper/VectorHelper.h"

Actor::Actor()
{

}
Actor::~Actor()
{
	pOuter.reset();
	pOuter = nullptr;

	for (auto&& pair : pComponents)
	{
		pair.second.reset();
		pair.second = nullptr;
	}
	pComponents.clear();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Actor::Tick(DirectX11& dx, float deltaTime)
{
	for (auto&& pair : pComponents)
	{
		if (pair.second != nullptr)
		{
			if (pair.second->GetTickEnabled())
			{
				pair.second->Tick(dx, deltaTime);
			}
		}
	}
}

void Actor::SetID(int inID)
{
	mID = inID;
}
int Actor::GetID() const
{
	return mID;
}

void Actor::RemoveComponent(const std::string& tag)
{
	auto& comp = pComponents.at(tag);
	comp.reset();
	comp = nullptr;

	pComponents.erase(tag);
}

// -----------------------------------
// Main : Transform
// -----------------------------------
FVector Actor::GetActorLocation()
{
	return mLocation;
}
void Actor::SetActorLocation(const FVector& in)
{
	mLocation = in;
}
void Actor::AddActorLocation(const FVector& in)
{
	mLocation += in;
}
FRotator Actor::GetActorRotation()
{
	return mRotation;
}
void Actor::SetActorRotation(const FRotator& in)
{
	mRotation = in;
}
FVector Actor::GetActorScale()
{
	return mScale;
}
void Actor::SetActorScale(const FVector& in)
{
	mScale = in;
}

FVector Actor::GetActorForwardVector()
{
	return FVector(
		std::cos(mRotation.pitch) * std::sin(mRotation.yaw),
		-std::sin(mRotation.pitch),
		std::cos(mRotation.pitch) * std::cos(mRotation.yaw)
	);
}
FVector Actor::GetActorUpVector()
{
	//return XMVector3NormalizeEst(
	//	{
	//		cos(rotation.y), 0, -sin(rotation.y)
	//	}
	//);

	return Vector::CrossProduct(
		FVector(0, 0, 1), GetActorForwardVector()
	);
}
FVector Actor::GetActorRightVector()
{
	return Vector::CrossProduct(
		GetActorUpVector(), GetActorForwardVector()
	);
}


// -----------------------------------
// Main : Util
// -----------------------------------
void Actor::SetOuter(std::shared_ptr<Object> inOuter)
{
	pOuter = inOuter;
}
std::shared_ptr<Object> Actor::GetOuter()
{
	return pOuter;
}

std::shared_ptr<Level> Actor::GetLevel()
{
	return GetTypedOuter<Level>();
}
std::shared_ptr<World> Actor::GetWorld()
{
	if (std::shared_ptr<Level> pLevel = GetLevel())
	{
		return pLevel->GetWorld();
	}
}