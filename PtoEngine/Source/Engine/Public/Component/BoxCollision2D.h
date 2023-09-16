#pragma once

#include "Component/CollisionComponent.h"
#include "Math/Rect.h"

#include <d2d1.h>

// ------------------------------------------------------
// Box Collision 2D
// ------------------------------------------------------
class BoxCollision2D : public CollisionComponent, public std::enable_shared_from_this<BoxCollision2D>
{
	friend class CollisionManager;
public:
	BoxCollision2D(Actor* inOwner);
	virtual ~BoxCollision2D();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;

	virtual bool InBoundingVolume(Actor* other) override;
	FRect GetRect();
protected:
	virtual void EnterVolume(std::shared_ptr<CollisionComponent> other) override;
	virtual void LeaveVolume(std::shared_ptr<CollisionComponent> other) override;

	virtual void ResolveBlock(std::shared_ptr<CollisionComponent> other) override;
public:

	// ----------------------------
	// Main : Debug
	// ----------------------------
	void DrawDebug(ID2D1RenderTarget* Rt2D);
protected:

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	ID2D1SolidColorBrush* pBrush = nullptr;
};