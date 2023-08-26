#pragma once

#include "Object/Object.h"

#include "Algorithm/Array.h"

#include "Engine/Timer.h"
#include "Engine/Vector.h"

#include "Engine/CollisionCollection.h"
#include "Engine/ObjectCollection.h"

class DirectX11;
class World;

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
class Level : public Object, public std::enable_shared_from_this<Level>
{
public:
	Level(DirectX11& dx);
	virtual ~Level();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void SetWorld(std::shared_ptr<World> in);
	virtual std::shared_ptr<World> GetWorld() override;

	CollisionCollection& GetCollisionCollection();
	ObjectCollection& GetObjectCollection();

	// ---------------------------
	// Main : Util
	// ---------------------------
	FVector GetStartPosition();
	void SetStartPosition(FVector in);

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	DirectX11* pDX = nullptr;

	std::shared_ptr<World> pOwningWorld = nullptr;

	CollisionCollection mCollisionCollection;
	ObjectCollection mObjectCollection;

	FVector mStartPosition;
};

