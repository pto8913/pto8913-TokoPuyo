
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

World* ComponentBase::GetWorld()
{
	return pOwner->GetWorld();
}
