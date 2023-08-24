
#include "Object/LayerObjectBase.h"

#include "GameSettings.h"

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
void LayerObject2DBase::ExecuteTasks(DirectX11& dx)
{
	Sprite::ExecuteTasks(dx);
}


// -----------------------------------
// Main : Transform
// -----------------------------------
void LayerObject2DBase::SetActorLocation(const FVector& in)
{
	Actor::SetActorLocation(in);

	DirectX::XMVECTOR vec = { 0,0,0 };
	DirectX::XMVectorSet(in.x, in.y, in.z, 0.f);
	Sprite::SetLocation(vec);
}
void LayerObject2DBase::SetActorRotation(const FRotator& in)
{
	Actor::SetActorRotation(in);

	DirectX::XMVECTOR vec;
	DirectX::XMVectorSet(in.roll, in.pitch, in.yaw, 0.f);
	Sprite::SetRotation(vec);
}
void LayerObject2DBase::SetActorScale(const FVector& in)
{
	Actor::SetActorScale(in);

	DirectX::XMVECTOR vec;
	DirectX::XMVectorSet(in.x, in.y, in.z, 0.f);
	Sprite::SetScale(vec);
}