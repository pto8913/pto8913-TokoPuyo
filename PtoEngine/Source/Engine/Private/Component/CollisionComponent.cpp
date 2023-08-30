
#include "Component/CollisionComponent.h"

CollisionComponent::CollisionComponent(Actor* inOwner)
	: ActorComponent(inOwner),
	mCollisionType(ECollisionType::Block)
{
}

CollisionComponent::~CollisionComponent()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
const ECollisionType& CollisionComponent::GetCollisionType() const
{
	return mCollisionType;
}
void CollisionComponent::SetCollisionType(const ECollisionType& in)
{
	mCollisionType = in;
}