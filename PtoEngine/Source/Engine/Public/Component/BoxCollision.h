#pragma once

#include "Component/CollisionComponent.h"
#include "Engine/Box.h"

// ------------------------------------------------------
// Box Collision
// ------------------------------------------------------
class BoxCollision : public CollisionComponent
{
	friend class CollisionCollection;
public:
	BoxCollision(Actor* inOwner);
	virtual ~BoxCollision();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaSec) override;

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
