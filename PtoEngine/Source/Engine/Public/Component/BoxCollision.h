#pragma once

#include "Component/CollisionComponent.h"
#include "Math/Box.h"

class Actor;

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
	virtual void EnterVolume(CollisionComponent* other) override;
	virtual void LeaveVolume(CollisionComponent* other) override;

	virtual void ResolveBlock(CollisionComponent* other) override;

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
};
