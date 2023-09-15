#pragma once

#include "Object/Actor.h"

#include "Actor2DTypes.h"

#include <chrono>

class DirectX11;
class SpriteComponent;

struct FActor2DSettings
{
public:
	FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const FVector2D& inSize, const Layer::EOrder& inOrder);
	FActor2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const Layer::EOrder& inOrder);

	std::wstring fileName;
	std::wstring tag;
	FVector2D size;
	Layer::EOrder sortOrder = Layer::UnOrder;
};

class Actor2D : public Actor
{
public:
	Actor2D(DirectX11& dx, const FActor2DSettings& Settings, const float& inUpdateTime = -1);

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(Actor2D).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
protected:
	// called per UpdateTime.
	virtual void Update(DirectX11& dx) {};
public:
	std::shared_ptr<SpriteComponent> GetSpriteComp();

	void SetSortOrder(Layer::EOrder inSortOrder);
	Layer::EOrder GetSortOrder() const;

	const FVector2D& Get2DIdx() const;
	void Set2DIdx(const FVector2D& in);

	// -----------------------------------
	// Main : Transform
	// -----------------------------------
	virtual FVector GetActorLocation() override;
	virtual void SetActorLocation(const FVector& in) override;
	virtual void AddActorLocation(const FVector& in) override;
	virtual void SetActorRotation(const FRotator& in) override;
	virtual void SetActorScale(const FVector& in) override;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::shared_ptr<SpriteComponent> pSpriteComp = nullptr;

	Layer::EOrder mSortOrder = Layer::UnOrder;
	FVector2D m2DIdx;

	// -------------------------
	// State : Tick
	// -------------------------
	float mUpdateTime = 1.f;

	using chrono = std::chrono::system_clock;
	chrono::time_point LastTime;
	chrono::duration DurationTime;
};