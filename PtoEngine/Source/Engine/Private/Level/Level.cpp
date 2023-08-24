
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
void Level::SetWorld(std::shared_ptr<World> in)
{
	pOwningWorld = in;
}
std::shared_ptr<World> Level::GetWorld()
{
	return pOwningWorld;
}

FVector Level::GetStartPosition()
{
	return mStartPosition;
}
void Level::SetStartPosition(FVector in)
{
	mStartPosition = in;
}