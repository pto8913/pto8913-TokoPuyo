
#include "Object/Actor2D.h"

#include "GameSettings.h"
#include "Core/AppSettings.h"

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
	auto c = GameSettings::GET_CELL(Settings.size.x, Settings.size.y);
	SetActorScale({ c.x, c.y, 0 });
	//SetActorScale({ Settings.size.x, Settings.size.y, 0 });
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

const FVector2D& Actor2D::Get2DIdx() const
{
	return m2DIdx;
}
void Actor2D::Set2DIdx(const FVector2D& in)
{
	m2DIdx = in;
}

// -----------------------------------
// Main : Transform
// -----------------------------------
FVector Actor2D::GetActorLocation()
{
	const auto vec = Actor::GetActorLocation();
	return {
		vec.x / AppSettings::aspectWidthRatio,
		vec.y / AppSettings::aspectHeightRatio,
		vec.z
	};
}
void Actor2D::SetActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
	SetLocation(vec);
	Actor::SetActorLocation(in);
}
void Actor2D::AddActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
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
	vec = XMVectorSet(in.x, in.y, in.z, 0.f);
	SetScale(vec);
	Actor::SetActorScale(in);
}