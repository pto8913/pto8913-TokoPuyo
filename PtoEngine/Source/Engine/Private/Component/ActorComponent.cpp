
#include "Component/ActorComponent.h"

#include "Object/Actor.h"

ActorComponent::ActorComponent(Actor* inOwner)
	: pOwner(inOwner)
{
}
ActorComponent::~ActorComponent()
{
	pOwner = nullptr;
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
Actor* ActorComponent::GetOwner()
{
	return pOwner;
}

std::shared_ptr<World> ActorComponent::GetWorld()
{
	return pOwner->GetWorld();
}
