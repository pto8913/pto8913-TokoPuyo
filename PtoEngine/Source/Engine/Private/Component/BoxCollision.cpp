
#include "Component/BoxCollision.h"

#include "Object/Actor.h"

#include "Helper/BoxHelper.h"

// ------------------------------------------------------
// Box Collision
// ------------------------------------------------------
BoxCollision::BoxCollision(Actor* inOwner)
	: CollisionComponent(inOwner)
{
}

BoxCollision::~BoxCollision()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
bool BoxCollision::InBoundingVolume(Actor* other)
{
	const auto box = GetBoundingBox();

	if (auto otherCollision = other->GetComponent<CollisionComponent>())
	{
		if (auto boxCollision = static_cast<BoxCollision*>(otherCollision))
		{
			if (Box::IsInBox(box, boxCollision->GetBoundingBox()))
			{
				return true;
			}
		}
		else
		{

		}
	}
	else
	{
		const auto target = other->GetActorLocation();
		if (Box::IsPointInBox(box, target))
		{
			return true;
		}
	}
	return false;
}
FBox BoxCollision::GetBoundingBox()
{
	const auto location = GetOwner()->GetActorLocation();
	const auto halfScale = GetOwner()->GetActorScale() / 2;
	return FBox(
		FVector(
			location.x - halfScale.x,
			location.y - halfScale.y,
			location.z + halfScale.z
		),
		FVector(
			location.x + halfScale.x,
			location.y + halfScale.y,
			location.z - halfScale.z
		)
	);
}
void BoxCollision::EnterVolume(CollisionComponent* other)
{
#if _DEBUG
	OutputDebugStringA("Enter Box Collision\n");
#endif
	switch (GetCollisionType())
	{
	case ECollisionType::Block:
		ResolveBlock(other);
		break;
	case ECollisionType::Overlap:
		break;
	default:
		break;
	}
}
void BoxCollision::LeaveVolume(CollisionComponent* other)
{

}

void BoxCollision::ResolveBlock(CollisionComponent* other)
{
	auto ownerSize = pOwner->GetActorScale();

	auto otherBox = static_cast<BoxCollision*>(other);

	const FBox& box1 = GetBoundingBox();
	const FBox& box2 = otherBox->GetBoundingBox();

	float diffX = (box1.leftTop.x + (box1.GetWidth() / 2)) - (box2.leftTop.x + (box2.GetWidth() / 2));
	float diffY = (box1.leftTop.y + (box1.GetHeight() / 2)) - (box2.leftTop.y + (box2.GetHeight() / 2));
	float diffZ = (box1.leftTop.z + (box1.GetWidth() / 2)) - (box2.leftTop.z + (box2.GetWidth() / 2));

	float x = 0, y = 0, z = 0;
	if (diffX > 0)
	{
		x = (box2.leftTop.x + box2.GetWidth()) - box1.leftTop.x;
	}
	else if (diffX < 0)
	{
		x = -(box1.leftTop.x + box1.GetWidth()) - box2.leftTop.x;
	}

	if (diffY > 0)
	{
		y = (box2.leftTop.y + box2.GetHeight()) - box1.leftTop.y;
	}
	else if (diffY < 0)
	{
		y = -(box1.leftTop.y + box1.GetHeight()) - box2.leftTop.y;
	}

	if (diffZ > 0)
	{
		z = (box2.leftTop.z + box2.GetWidth()) - box1.leftTop.z;
	}
	else if (diffZ < 0)
	{
		z = -(box1.leftTop.z + box1.GetWidth()) - box2.leftTop.z;
	}

	pOwner->AddActorLocation(FVector(x, y, z));
}

