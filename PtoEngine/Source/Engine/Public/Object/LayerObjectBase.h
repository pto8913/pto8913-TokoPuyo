#pragma once

#include "Object/Sprite.h"
#include "Object/Actor.h"

#include <chrono>

class DirectX11;

struct FLayerObject2DSettings
{
public:
	FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag, const DirectX::XMFLOAT2& inSize);
	FLayerObject2DSettings(const std::wstring& inFileName, const std::wstring& inTag);

	std::wstring fileName;
	std::wstring tag;
	DirectX::XMFLOAT2 size;
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
	const DirectX::XMFLOAT2& GetSize() const noexcept;
protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	std::wstring tag = L"";
	DirectX::XMFLOAT2 size;

	float UpdateTime = 1.f;

	using chrono = std::chrono::system_clock;
	chrono::time_point LastTime;
	chrono::duration DurationTime;
};