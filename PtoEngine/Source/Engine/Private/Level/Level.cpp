
#include "Level/Level.h"

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
Level::Level(DirectX11& dx)
	: pDX(&dx)
{
	SetTickEnabled(true);
}
Level::~Level()
{
	pDX = nullptr;

	pOwningWorld.reset();
	pOwningWorld = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level::Tick(DirectX11& dx, float deltaSec)
{
	Object::Tick(dx, deltaSec);

	//mCollisionCollection.Tick();
	mObjectCollection.Tick(dx, deltaSec);
}

void Level::SetWorld(std::shared_ptr<World> in)
{
	pOwningWorld = in;
}
std::shared_ptr<World> Level::GetWorld()
{
	return pOwningWorld;
}

CollisionCollection& Level::GetCollisionCollection()
{
	return mCollisionCollection;
}
ObjectCollection& Level::GetObjectCollection()
{
	return mObjectCollection;
}

// ---------------------------
// Main : Util
// ---------------------------
FVector Level::GetStartPosition()
{
	return mStartPosition;
}
void Level::SetStartPosition(FVector in)
{
	mStartPosition = in;
}