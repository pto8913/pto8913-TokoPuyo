
#include "Object/Actor.h"

#include "Level/Level.h"

#include "Engine/World.h"

Actor::Actor()
{

}
Actor::~Actor()
{
	pOuter.reset();
	pOuter = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------

// -----------------------------------
// Main : Util
// -----------------------------------
void Actor::SetOuter(std::shared_ptr<Object> inOuter)
{
	pOuter = inOuter;
}
std::shared_ptr<Object> Actor::GetOuter()
{
	return pOuter;
}

std::shared_ptr<Level> Actor::GetLevel()
{
	return GetTypedOuter<Level>();
}
std::shared_ptr<World> Actor::GetWorld()
{
	if (std::shared_ptr<Level> pLevel = GetLevel())
	{
		return pLevel->GetWorld();
	}
}