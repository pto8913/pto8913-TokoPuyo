
#include "Actor/Actor2D.h"

#include "EngineSettings.h"

#include "Component/SpriteComponent.h"

using namespace DirectX;

FActor2DSettings::FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize, const Layer::EOrder& inOrder)
	: fileName(inFileName), tag(inTag), size(inSize), sortOrder(inOrder)
{
}

FActor2DSettings::FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const Layer::EOrder& inOrder)
	: fileName(inFileName), tag(inTag), size(EngineSettings::GetGameScreen2DCellSize()), sortOrder(inOrder)
{
}

Actor2D::Actor2D(DirectX11& dx, const FActor2DSettings& Settings, const float& inUpdateTime)
	: DurationTime(0),
	mUpdateTime(inUpdateTime)
{
	pSpriteComp = AddComponent<SpriteComponent>(
		"sprite", 
		this, 
		dx, 
		Settings.fileName,
		Settings.tag,
		XMFLOAT2(Settings.size.x, Settings.size.y)
	);

	auto c = EngineSettings::GETCELL(Settings.size);
	SetActorScale({ c.x, c.y, 0 });
	LastTime = chrono::now();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void Actor2D::Tick(DirectX11& dx, float deltaTime)
{
	if (GetTickEnabled())
	{
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
}

std::shared_ptr<SpriteComponent> Actor2D::GetSpriteComp()
{
	return pSpriteComp;
}
void Actor2D::SetSortOrder(Layer::EOrder inSortOrder)
{
	mSortOrder = inSortOrder;
}
Layer::EOrder Actor2D::GetSortOrder() const
{
	return mSortOrder;
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
	const auto ratio = EngineSettings::GetWindowAspectRatio();
	return {
		vec.x / ratio.x,
		vec.y / ratio.y,
		vec.z
	};
}
void Actor2D::SetActorLocation(const FVector& in)
{
	pSpriteComp->SetActorLocation(in);
	Actor::SetActorLocation(in);
}
void Actor2D::AddActorLocation(const FVector& in)
{
	pSpriteComp->AddActorLocation(in);
	Actor::AddActorLocation(in);
}
void Actor2D::SetActorRotation(const FRotator& in)
{
	pSpriteComp->SetActorRotation(in);
	Actor::SetActorRotation(in);
}
void Actor2D::SetActorScale(const FVector& in)
{
	pSpriteComp->SetActorScale(in);
	Actor::SetActorScale(in);
}