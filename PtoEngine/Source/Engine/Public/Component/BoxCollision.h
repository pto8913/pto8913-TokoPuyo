#pragma once

#include "Component/CollisionComponent.h"
#include "Engine/Vector.h"


// ------------------------------------------------------
// FBox
// ------------------------------------------------------
struct FBox
{
public:
	FBox(const FVector& left, const FVector& right);
	FBox(FVector&& left, FVector&& right);

	FVector leftTop;
	FVector rightBottom;

	bool IsPointInBox(const FVector& point) const;
	bool IsInBox(const FBox& box) const;
	float GetWidth() const;
	float GetHeight() const;
};

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