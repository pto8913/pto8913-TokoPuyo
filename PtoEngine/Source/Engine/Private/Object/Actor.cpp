
#include "Object/Actor.h"

#include "Framework/World.h"
#include "Framework/Level/Level.h"

#include "Component/ComponentBase.h"

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

void Actor::Construct()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Actor::BeginPlay(DirectX11& dx)
{
	for (auto&& pair : pComponents)
	{
		if (pair.second != nullptr)
		{
			pair.second->BeginPlay(dx);
		}
	}
}
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

const Layer::EActorLayer& Actor::GetLayer() const
{
	return mLayer;
}
void Actor::SetLayer(const Layer::EActorLayer& in)
{
	mLayer = in;
}

// -----------------------------------
// Main : Component
// -----------------------------------
void Actor::RemoveComponent(const std::string& tag)
{
	if (pComponents.contains(tag))
	{
		auto& comp = pComponents.at(tag);
		if (comp)
		{
			comp.reset();
			comp = nullptr;
		}

		pComponents.erase(tag);
	}
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
	return nullptr;
}