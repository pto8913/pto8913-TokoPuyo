#pragma once

#include "Component/CollisionComponent.h"
#include "Math/Box.h"

// ------------------------------------------------------
// Box Collision
// ------------------------------------------------------
class BoxCollision : public CollisionComponent
{
	friend class CollisionManager;
public:
	BoxCollision(Actor* inOwner);
	virtual ~BoxCollision();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual bool InBoundingVolume(Actor* other) override;
	FBox GetBoundingBox();
protected:
	virtual void EnterVolume(std::shared_ptr<CollisionComponent> other) override;
	virtual void LeaveVolume(std::shared_ptr<CollisionComponent> other) override;

	virtual void ResolveBlock(std::shared_ptr<CollisionComponent> other) override;

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
};
