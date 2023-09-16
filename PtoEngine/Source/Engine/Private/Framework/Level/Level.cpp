
#include "Framework/Level/Level.h"

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
Level::Level(DirectX11& dx)
	: pDX(&dx)
{
	SetObjectManager();
}
Level::~Level()
{
	pDX = nullptr;

	pOwningWorld = nullptr;

	pObjectManager.reset();
	pObjectManager = nullptr;
}

void Level::SetObjectManager()
{
	if (pObjectManager == nullptr)
	{
		pObjectManager = std::make_shared<ObjectManager>();
	}
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Level::Tick(DirectX11& dx, float deltaSec)
{
	Object::Tick(dx, deltaSec);

	//mCollisionManager.Tick();
	if (pObjectManager != nullptr)
	{
		pObjectManager->Tick(dx, deltaSec);
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

CollisionManager& Level::GetCollisionManager()
{
	return mCollisionManager;
}

ObjectManager* Level::GetObjectManager()
{
	return pObjectManager.get();
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