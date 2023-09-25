
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

	pOwningWorld.reset();
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

	if (pObjectManager != nullptr)
	{
		pObjectManager->Tick(dx, deltaSec);
	}
}

void Level::SetWorld(std::shared_ptr<World> in)
{
	pOwningWorld = in;
}
std::shared_ptr<World> Level::GetWorld()
{
	return pOwningWorld;
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