
#include "Object/LayerObjectBase.h"

#include "GameSettings.h"

using namespace DirectX;

FLayerObject2DSettings::FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize)
	: fileName(inFileName), tag(inTag), size(inSize)
{
}

FLayerObject2DSettings::FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag)
	: fileName(inFileName), tag(inTag), size({ GameSettings::CELL, GameSettings::CELL })
{
}

LayerObject2DBase::LayerObject2DBase(DirectX11& dx, const FLayerObject2DSettings& Settings, const float& inUpdateTime)
	: Sprite(
		dx,
		Settings.fileName,
		Settings.tag,
		{ Settings.size.x, Settings.size.y },
		{ 0, 0 }
	),
	DurationTime(0),
	mUpdateTime(inUpdateTime)
{
	SetActorScale(FVector(Settings.size.x, Settings.size.y, 0));
	LastTime = chrono::now();
}

// ------------------------------------------------------
// Main
// ------------------------------------------------------
void LayerObject2DBase::Tick(DirectX11& dx, float deltaTime)
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

// -----------------------------------
// Main : Transform
// -----------------------------------
void LayerObject2DBase::SetActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	SetLocation(vec);
	Actor::SetActorLocation(in);
}
void LayerObject2DBase::AddActorLocation(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	vec = DirectX::XMVectorAdd(GetLocation(), vec);
	SetLocation(vec);
	Actor::AddActorLocation(in);
}
void LayerObject2DBase::SetActorRotation(const FRotator& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.roll, in.pitch, in.yaw, 0.f);
	SetRotation(vec);
	Actor::SetActorRotation(in);
}
void LayerObject2DBase::SetActorScale(const FVector& in)
{
	DirectX::XMVECTOR vec({ 0.f,0.f,0.f, 0.f });
	vec = XMVectorSet(in.x, in.z, in.y, 0.f);
	SetScale(vec);
	Actor::SetActorScale(in);
}