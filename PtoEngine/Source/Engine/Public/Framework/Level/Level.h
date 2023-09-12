#pragma once

#include "Object/Object.h"

#include "Algorithm/Array.h"

#include "Engine/Timer.h"
#include "Math/Vector.h"

#include "Framework/Level/CollisionCollection.h"
#include "Framework/Level/ObjectCollection.h"

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

protected:
	virtual void SetObjectCollection();

public:
	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaSec) override;

	void SetWorld(World* in);
	virtual World* GetWorld() override;

	CollisionCollection& GetCollisionCollection();
	
	ObjectCollection* GetObjectCollection();

public:
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

	World* pOwningWorld = nullptr;

	CollisionCollection mCollisionCollection;
	std::shared_ptr<ObjectCollection> pObjectCollection = nullptr;

	FVector mStartPosition;
};
