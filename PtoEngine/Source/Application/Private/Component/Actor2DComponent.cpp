
#include "Component/Actor2DComponent.h"

Actor2DComponent::Actor2DComponent(Actor* inOwner, const Layer::EOrder& inSortOrder)
	: ComponentBase(inOwner),
	mSortOrder(inSortOrder)
{
}
Actor2DComponent::~Actor2DComponent()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Actor2DComponent::SetSortOrder(Layer::EOrder inSortOrder)
{
	mSortOrder = inSortOrder;
}
Layer::EOrder Actor2DComponent::GetSortOrder() const
{
	return mSortOrder;
}

const FVector2D& Actor2DComponent::Get2DIdx() const
{
	return m2DIdx;
}
void Actor2DComponent::Set2DIdx(const FVector2D& in)
{
	m2DIdx = in;
}