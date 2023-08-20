
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
		{0, 0}
	), 
	DurationTime(0),
	mUpdateTime(inUpdateTime),
	mSize(Settings.size)
{
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

const FVector2D& LayerObject2DBase::GetSize() const noexcept
{
	return mSize;
}
const FVector2D& LayerObject2DBase::GetPosition() const noexcept
{
	return mPosition;
}