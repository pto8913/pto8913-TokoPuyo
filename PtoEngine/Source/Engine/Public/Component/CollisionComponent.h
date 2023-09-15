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
	friend class CollisionCollection;
public:
	CollisionComponent(Actor* inOwner);
	virtual ~CollisionComponent();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(CollisionComponent).name() + std::string("_") + std::to_string(mID);
	}
	virtual bool InBoundingVolume(Actor* other) = 0;

	const ECollisionType& GetCollisionType() const;
	void SetCollisionType(const ECollisionType&);

protected:
	virtual void EnterVolume(std::shared_ptr<CollisionComponent> other) {};
	virtual void LeaveVolume(std::shared_ptr<CollisionComponent> other) {};

	virtual void ResolveBlock(std::shared_ptr<CollisionComponent> other) {};
	virtual void ResolveOverlap(std::shared_ptr<CollisionComponent> other) {};

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ECollisionType mCollisionType;
};