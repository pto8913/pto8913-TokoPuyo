#pragma once

#include "Framework/Level/Level.h"

#include "Actor/Ground/GroundTypes.h"

#include "Actor/Actor2DTypes.h"

class Actor2D;

class GroundBase;
class EventBase;
class ItemBase;
class CharacterBase;

// ------------------------------------------------------------------------------------------------------------
// Level 2D
// ------------------------------------------------------------------------------------------------------------
class Level2D : public Level
{
public:
	Level2D(DirectX11& dx);
	virtual ~Level2D();
	virtual void SetObjectCollection() override;

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual std::string GetName() const override
	{
		using namespace std::string_literals;
		return typeid(Level2D).name() + std::string(" : ") + "#"s + std::to_string(mID);
	}
	virtual void BeginPlay(DirectX11& dx) override;
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void Generate(DirectX11& dx);
protected:
	virtual void GenerateGroundLayer() = 0;
public:
	virtual void Init(const int& x, const int& y);

	// --------------------------
	// Main : Utils
	// --------------------------
	const int& GetWidth() const noexcept;
	const int& GetHeight() const noexcept;

	bool IsInWorld(const float& x, const float& y) const noexcept;

	DirectX::XMFLOAT2 WorldToScreen(const float& x, const float& y, const FVector& size);
protected:
	virtual void SetSpriteLocation(Actor2D* sprite, const float& worldX, const float& worldY);
	Actor2D* GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const Layer::EActorLayer& inLayer) const;
public:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	int width = 0;
	int height = 0;
protected:
	bool bInitialized = false;

};