
#include "Component/ComponentBase.h"

#include "Object/Actor.h"

ComponentBase::ComponentBase(Actor* inOwner)
	: pOwner(inOwner)
{
}
ComponentBase::~ComponentBase()
{
	pOwner = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
Actor* ComponentBase::GetOwner()
{
	return pOwner;
}

std::shared_ptr<World> ComponentBase::GetWorld()
{
	return pOwner->GetWorld();
}
