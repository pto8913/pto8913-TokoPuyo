
#include "Framework/Level/Level.h"

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
Level::Level(DirectX11& dx)
	: pDX(&dx)
{
	SetObjectCollection();
}
Level::~Level()
{
	pDX = nullptr;

	pOwningWorld = nullptr;

	pObjectCollection.reset();
	pObjectCollection = nullptr;
}

void Level::SetObjectCollection()
{
	if (pObjectCollection == nullptr)
	{
		pObjectCollection = std::make_shared<ObjectCollection>();
	}
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level::Tick(DirectX11& dx, float deltaSec)
{
	Object::Tick(dx, deltaSec);

	//mCollisionCollection.Tick();
	if (pObjectCollection != nullptr)
	{
		pObjectCollection->Tick(dx, deltaSec);
	}
}

void Level::SetWorld(World* in)
{
	pOwningWorld = in;
}
World* Level::GetWorld()
{
	return pOwningWorld;
}

std::shared_ptr<ObjectCollection>& Level::GetObjectCollection()
{
	return pObjectCollection;
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