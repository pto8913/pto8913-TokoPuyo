
#include "Component/BoxCollision2D.h"

#include "Object/Actor.h"

#include "Helper/RectHelper.h"

#include "Math/Rect.h"

#if _DEBUG
#include "Framework/World.h"
#include "UI/HUD.h"
#endif

#include "EngineSettings.h"

// ------------------------------------------------------
// Box Collision
// ------------------------------------------------------
BoxCollision2D::BoxCollision2D(Actor* inOwner)
	: CollisionComponent(inOwner)
{
}

BoxCollision2D::~BoxCollision2D()
{
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void BoxCollision2D::BeginPlay(DirectX11& dx)
{
	CollisionComponent::BeginPlay(dx);
#if _DEBUG

	auto hud = GetWorld()->GetHUD();
	hud->AddBoxDebug(shared_from_this());
#endif
}

bool BoxCollision2D::InBoundingVolume(Actor* other)
{
	const auto rect = GetRect();

	if (auto otherCollision = other->GetComponent<CollisionComponent>())
	{
		if (auto boxCollision = static_pointer_cast<BoxCollision2D>(otherCollision))
		{
			if (RectHelper::IsInRect(rect, boxCollision->GetRect()))
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
		if (RectHelper::IsPointInRect(rect, target))
		{
			return true;
		}
	}
	return false;
}
FRect BoxCollision2D::GetRect()
{
	const auto location = GetOwner()->GetActorLocation();
	const auto halfScale = GetOwner()->GetActorScale() / 2;
	//OutputDebugStringA(("Box collision owner scale " + halfScale.ToString() + "\n").c_str());
	//return FRect(
	//	location.x - halfScale.x,
	//	location.y - halfScale.y,
	//	location.x + halfScale.x,
	//	location.y + halfScale.y-2
	//);

	const auto scale = GetOwner()->GetActorScale();
	const auto ratio = EngineSettings::GetWindowAspectRatio();
	return FRect(
		location.x,
		location.y,
		location.x + scale.x / ratio.x,
		location.y + scale.y / ratio.y
	);
}
void BoxCollision2D::EnterVolume(std::shared_ptr<CollisionComponent> other)
{
	OutputDebugStringA("Enter Box Collision\n");
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
void BoxCollision2D::LeaveVolume(std::shared_ptr<CollisionComponent> other)
{

}

void BoxCollision2D::ResolveBlock(std::shared_ptr<CollisionComponent> other)
{
	auto ownerSize = pOwner->GetActorScale();

	auto otherBox = static_pointer_cast<BoxCollision2D>(other);

	const FRect& box1 = GetRect();
	const FRect& box2 = otherBox->GetRect();

	float diffX = (box1.left + (box1.GetWidth() / 2)) - (box2.left + (box2.GetWidth() / 2));
	float diffY = (box1.top + (box1.GetHeight() / 2)) - (box2.top + (box2.GetHeight() / 2));

	float x = 0, y = 0;
	if (fabs(diffX) > fabs(diffY))
	{
		if (diffX > 0)
		{
			x = box2.left + box2.GetWidth() - box1.left;
		}
		else if (diffX < 0)
		{
			x = -(box1.left + box1.GetWidth() - box2.left);
		}
	}
	else
	{
		if (diffY > 0)
		{
			y = box2.top + box2.GetHeight() - box1.top;
		}
		else if (diffY < 0)
		{
			y = -(box1.top + box1.GetHeight() - box2.top);
		}
	}
	pOwner->AddActorLocation(FVector(x, y, 0));
}

// ----------------------------
// Main : Debug
// ----------------------------
void BoxCollision2D::DrawDebug(ID2D1RenderTarget* Rt2D)
{
#if _DEBUG
	if (pBrush == nullptr)
	{
		Rt2D->CreateSolidColorBrush(
			D2D1::ColorF(
				1.f, 0.f, 1.f, 1.f
			),
			&pBrush
		);
	}
	Rt2D->DrawRectangle(
		RectHelper::ConvertRectToD2D(GetRect()),
		pBrush
	);
#endif
}