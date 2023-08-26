#pragma once

#include "Object/Sprite.h"
#include "Object/Actor.h"

#include "Actor2DTypes.h"

#include <chrono>

class DirectX11;

struct FActor2DSettings
{
public:
	FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize);
	FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag);

	std::wstring fileName;
	std::wstring tag;
	FVector2D size;
};

class Actor2D : public Sprite, public Actor
{
public:
	Actor2D(DirectX11& dx, const FActor2DSettings& Settings, const float& inUpdateTime = -1);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;
protected:
	// called per UpdateTime.
	virtual void Update(DirectX11& dx) {};
public:
	void SetSortOrder(Layer::EOrder inSortOrder);
	Layer::EOrder GetSortOrder() const;

	const EActor2DLayer& GetLayer() const;
	void SetLayer(const EActor2DLayer& in);

	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual void SetActorLocation(const FVector& in) override;
	virtual void AddActorLocation(const FVector& in) override;
	virtual void SetActorRotation(const FRotator& in) override;
	virtual void SetActorScale(const FVector& in) override;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	Layer::EOrder mSortOrder = Layer::UnOrder;
	EActor2DLayer mLayer;

	// -------------------------
	// State : Tick
	// -------------------------
	float mUpdateTime = 1.f;

	using chrono = std::chrono::system_clock;
	chrono::time_point LastTime;
	chrono::duration DurationTime;
};