#pragma once

#include "Object/Sprite.h"
#include "Object/Actor.h"

#include <chrono>

class DirectX11;

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnTransformChanged, );

struct FLayerObject2DSettings
{
public:
	FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize);
	FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag);

	std::wstring fileName;
	std::wstring tag;
	FVector2D size;
};

class LayerObject2DBase : public Sprite, public Actor
{
public:
	LayerObject2DBase(DirectX11& dx, const FLayerObject2DSettings& Settings, const float& inUpdateTime = -1);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;
protected:
	// called per UpdateTime.
	virtual void Update(DirectX11& dx) {};
	virtual void ExecuteTasks(DirectX11& dx) override final;
public:
	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual void SetActorLocation(const FVector& in) override;
	virtual void SetActorRotation(const FRotator& in) override;
	virtual void SetActorScale(const FVector& in) override;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::wstring tag = L"";

	// -------------------------
	// State : Tick
	// -------------------------
	float mUpdateTime = 1.f;

	using chrono = std::chrono::system_clock;
	chrono::time_point LastTime;
	chrono::duration DurationTime;
};