
#include "Object/Actor2D.h"

#include "GameSettings.h"

using namespace DirectX;

FActor2DSettings::FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
	: fileName(inFileName), tag(inTag), size(inSize)
{
}

FActor2DSettings::FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag)
	: fileName(inFileName), tag(inTag), size({ GameSettings::CELL, GameSettings::CELL })
{
}

Actor2D::Actor2D(DirectX11& dx, const FActor2DSettings& Settings, const float& inUpdateTime)
	: Sprite(
		dx,
		Settings.fileName,
		Settings.tag,
		{ Settings.size.x, Settings.size.y }
	),
	DurationTime(0),
	mUpdateTime(inUpdateTime)
{
	SetActorScale(FVector(Settings.size.x, 0, Settings.size.y));
	LastTime = chrono::now();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Actor2D::Tick(DirectX11& dx, float deltaTime)
{
	ExecuteTasks(dx);
	Actor::Tick(dx, deltaTime);

	if (mUpdateTime != -1.f)
	{
		DurationTime = chrono::now() - LastTime;
		if (std::chrono::duration_cast<std::chrono::microseconds>(DurationTime).count() / 1000 >= mUpdateTime)
		{
			LastTime = chrono::now();
			Update(dx);
		}
	}
}

void Actor2D::SetSortOrder(Layer::EOrder inSortOrder)
{
	mSortOrder = inSortOrder;
}
Layer::EOrder Actor2D::GetSortOrder() const
{
	return mSortOrder;
}

const EActor2DLayer& Actor2D::GetLayer() const
{
	return mLayer;
}
void Actor2D::SetLayer(const EActor2DLayer& in)
{
	mLayer = in;
}

// -----------------------------------
// Main : Transform
// -----------------------------------
void Actor2D::SetActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	SetLocation(vec);
	Actor::SetActorLocation(in);
}
void Actor2D::AddActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	vec = DirectX::XMVectorAdd(GetLocation(), vec);
	SetLocation(vec);
	Actor::AddActorLocation(in);
}
void Actor2D::SetActorRotation(const FRotator& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.roll, in.pitch, in.yaw, 0.f);
	SetRotation(vec);
	Actor::SetActorRotation(in);
}
void Actor2D::SetActorScale(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	SetScale(vec);
	Actor::SetActorScale(in);
}