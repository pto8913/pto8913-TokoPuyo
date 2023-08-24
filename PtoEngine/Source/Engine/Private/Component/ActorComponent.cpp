
#include "Component/ActorComponent.h"

ActorComponent::ActorComponent(std::shared_ptr<Actor> inOwner)
	: pOwner(inOwner)
{
}
ActorComponent::~ActorComponent()
{
	pOwner.reset();
	pOwner = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
std::shared_ptr<Actor> ActorComponent::GetOwner()
{
	return pOwner;
}

