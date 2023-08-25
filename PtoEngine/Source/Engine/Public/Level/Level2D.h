#pragma once

#include "Level.h"

#include "Layer/Layer.h"

class LayerObject2DBase;

class GroundBase;
class EventBase;
class ItemBase;
class CharacterBase;

class WorldTimer;

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
	virtual bool MoveCenter(const float& x, const float& y);
protected:
	void UpdateSpriteOffset();

public:
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
	FVector2D GetCenter() const noexcept;

	bool IsInScreen(const int& x, const int& y) const noexcept;
	bool IsInWorld(const int& x, const int& y) const noexcept;
	bool IsEmptyGround(const int& x, const int& y) const noexcept;

	/* move  */
	bool CanMove(const int& x, const int& y) const noexcept;

	DirectX::XMFLOAT2 WorldToScreen(const int& x, const int& y, const FVector2D& size);

	// --------------------------
	// Main : Utils : Ground
	// --------------------------
	void SetGroundLayerID(const EGroundId& groundType, const FVector2D& pos);
	void SetGroundLayerID(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal = false, INT16 inConstantXY2 = -1);
	const std::shared_ptr<GroundBase>& GetGroundLayerID(const int& x, const int& y) const;

	// --------------------------
	// Main : Utils : Event
	// --------------------------
	void SetEventLayerID(const EEventId& eventType, const FVector2D& pos);
	void SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const FVector2D& pos);
	void SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const FVector2D& pos);
	const std::shared_ptr<EventBase>& GetEventLayerID(const int& x, const int& y) const;

	// --------------------------
	// Main : Utils : Item
	// --------------------------
	void SetItemLayerID(const EItemId& itemType, const FVector2D& pos);
	const std::shared_ptr<ItemBase>& GetItemLayerID(const int& x, const int& y) const;

	// --------------------------
	// Main : Utils : Character
	// --------------------------
	void SetCharacterLayerID(const ECharacterId& characterType, const FVector2D& pos);
	const std::shared_ptr<CharacterBase>& GetCharacterLayerID(const int& x, const int& y) const;
	const std::shared_ptr<CharacterBase>& GetCharacter2LayerID(const int& x, const int& y) const;

public:
	// --------------------------
	// Main : Debug
	// --------------------------
	void ShowTiles();

	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	TArray<TArray<std::shared_ptr<GroundBase>>> GroundLayer;
	TArray<TArray<std::shared_ptr<EventBase>>> EventLayer;

	TArray<TArray<std::shared_ptr<ItemBase>>> ItemLayer;
	//TArray<TArray<std::shared_ptr<CharacterBase>>> BuildingLayer;

	TArray<TArray<std::shared_ptr<CharacterBase>>> CharacterLayer;
	TArray<TArray<std::shared_ptr<CharacterBase>>> Character2Layer;
	//TArray<TArray<std::shared_ptr<CharacterBase>>> EffectLayer;

	int width = 0;
	int height = 0;
protected:
	bool bInitialized = false;
	EGroundType mGroundType = EGroundType::Cave;

	// --------------------------
	// State : Display
	// --------------------------

	FVector2D mCenter;
	int screenLeftX = 0;
	int screenLeftY = 0;
	int screenRightX = 0;
	int screenRightY = 0;

	// --------------------------
	// State : WorldTimer
	// --------------------------
	std::unique_ptr<WorldTimer> pWorldTimer = nullptr;
	float worldTimerSpeed = 1.f;
};