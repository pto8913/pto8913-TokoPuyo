#pragma once

#include "Level.h"

#include "Object/Ground/GroundTypes.h"
#include "Object/Event/EventTypes.h"
#include "Object/Item/ItemTypes.h"
#include "Object/Character/CharacterTypes.h"
#include "Object/Building/BuildingTypes.h"
#include "Object/Effect/EffectTypes.h"

class Actor2D;

class GroundBase;
class EventBase;
class ItemBase;
class CharacterBase;

// ------------------------------------------------------------------------------------------------------------
// Level 2D
// ------------------------------------------------------------------------------------------------------------
enum class EDirection
{
	Left,
	Right,
	Up,
	Down,
	InValid
};

class Level2D : public Level
{
public:
	Level2D(DirectX11& dx);
	virtual ~Level2D();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	virtual void BeginPlay(DirectX11& dx) override;
	void Generate(DirectX11& dx);
	virtual void Tick(DirectX11& dx, float deltaTime) override;

	void SetGroundType(const EGroundType& inGroundType);

protected:
	virtual void GenerateGroundLayer() = 0;
	virtual void GenerateEventLayer() = 0;
	virtual void GenerateItemLayer() {}
	virtual void GenerateBuildingLayer() {}
	virtual void GenerateCharacterLayer() {}
	virtual void GenerateEffectLayer() {}

public:
	void Init(const int& x, const int& y);
	void Activate();
	void Deactivate();
	virtual bool MoveCenter(const int& x, const int& y);

	virtual void Clear();
	template<typename T>
	void Clear(TArray<TArray<T>>& in)
	{
		for (auto&& elem : in)
		{
			for (auto&& layer : elem)
			{
				layer.reset();
			}
			elem.Clear();
		}
		in.Clear();
	}

	// --------------------------
	// Main : Utils
	// --------------------------
	const int& GetWidth() const noexcept;
	const int& GetHeight() const noexcept;

	bool IsInScreen(const int& x, const int& y, const int& buffer = 0) const noexcept;
	bool IsInScreen(const float& x, const float& y) const noexcept;
	bool IsInWorld(const int& x, const int& y) const noexcept;

	DirectX::XMFLOAT2 WorldToScreen(const int& x, const int& y, const FVector& size);
	void ScreenToWorld(const FVector& world, const FVector& size, int& x, int& y) const;

	// --------------------------
	// Main : Utils : Ground
	// --------------------------
	void SetGroundLayerID(const EGroundId& groundType, const float& worldX, const float& worldY);
	void SetGroundLayerID(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal = false, INT16 inConstantXY2 = -1);
	std::shared_ptr<GroundBase> GetGroundLayer(const int& worldX, const int& worldY) const;

	// --------------------------
	// Main : Utils : Event
	// --------------------------
	void SetEventLayerID(const EEventId& eventType, const float& worldX, const float& worldY);
	void SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const float& worldX, const float& worldY);
	void SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const float& worldX, const float& worldY);
	std::shared_ptr<EventBase> GetEventLayer(const int& worldX, const int& worldY) const;

	// --------------------------
	// Main : Utils : Item
	// --------------------------
	void SetItemLayerID(const EItemId& itemType, const float& worldX, const float& worldY);
	std::shared_ptr<ItemBase> GetItemLayer(const int& worldX, const int& worldY) const;

	// --------------------------
	// Main : Utils : Character
	// --------------------------
	void SetCharacterLayerID(const ECharacterId& characterType, const float& worldX, const float& worldY);
	std::shared_ptr<CharacterBase> GetCharacterLayer(const int& worldX, const int& worldY) const;

protected:
	void SetSpriteLocation(std::shared_ptr<Actor2D> sprite, const float& worldX, const float& worldY);
	std::shared_ptr<Actor2D> GetLayer(const int& worldX, const int& worldY, const Layer::EOrder& inOrder, const EActor2DLayer& inLayer) const;
public:
	// --------------------------
	// Main : Debug
	// --------------------------
	void ShowTiles();

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	int width = 0;
	int height = 0;
protected:
	bool bInitialized = false;
	EGroundType mGroundType = EGroundType::Cave;

	// --------------------------
	// State : Display
	// --------------------------

	int centerX = 0;
	int centerY = 0;
	int screenLeftX = 0;
	int screenLeftY = 0;
	int screenRightX = 0;
	int screenRightY = 0;
};