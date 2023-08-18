
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

Level* Actor::GetLevel()
{
	return GetTypedOuter<Level>();
}
std::shared_ptr<World> Actor::GetWorld()
{
	if (Level* pLevel = GetLevel())
	{
		return pLevel->GetWorld();
	}
}