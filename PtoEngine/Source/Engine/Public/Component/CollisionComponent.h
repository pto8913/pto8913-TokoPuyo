#pragma once

#include "Component/ActorComponent.h"

enum class ECollisionType
{
	Block,
	Overlap,
	Ignore,
};

class CollisionComponent : public ActorComponent
{
	friend class CollisionManager;
public:
	CollisionComponent(Actor* inOwner);
	virtual ~CollisionComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return "CollisionComponent" + "#"s + std::to_string(mID);
	}
	virtual bool InBoundingVolume(Actor* other) = 0;

	const ECollisionType& GetCollisionType() const;
	void SetCollisionType(const ECollisionType&);

protected:
	virtual void EnterVolume(CollisionComponent* other) {};
	virtual void LeaveVolume(CollisionComponent* other) {};

	virtual void ResolveBlock(CollisionComponent* other) {};
	virtual void ResolveOverlap(CollisionComponent* other) {};

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ECollisionType mCollisionType;
};