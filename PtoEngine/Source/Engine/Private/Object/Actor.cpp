
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
	OnDestroyed.ClearBind();

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
void Actor::EndPlay()
{
	OnDestroyed.Broadcast(shared_from_this());
}
void Actor::Tick(DirectX11& dx, float deltaTime)
{
	if (!IsPendingKill())
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
}
void Actor::DestroyActor()
{
	MarkPendingKill();
}

void Actor::SetID(int inID)
{
	mID = inID;
}
int Actor::GetID() const
{
	return mID;
}

// -----------------------------------
// Main : Component
// -----------------------------------
void Actor::RemoveComponent(const std::string& tag)
{
	if (!IsPendingKill())
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
void Actor::SetOuter(Object* inOuter)
{
	pOuter = inOuter;
}
Object* Actor::GetOuter()
{
	return pOuter;
}

Level* Actor::GetLevel()
{
	if (!IsPendingKill())
	{
		return GetTypedOuter<Level>();
	}
	return nullptr;
}
World* Actor::GetWorld()
{
	if (!IsPendingKill())
	{
		if (auto pLevel = GetLevel())
		{
			return pLevel->GetWorld();
		}
	}
	return nullptr;
}