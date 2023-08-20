#pragma once

#include "Object/Object.h"

#include "Algorithm/Array.h"

#include "Engine/Timer.h"
#include "Engine/Vector.h"

#include "Layer/Layer.h"

class DirectX11;
class World;

class LayerObject2DBase;
class Layer;

class GroundBase;
class EventBase;
class ItemBase;
class CharacterBase;

class WorldTimer;

// ------------------------------------------------------------------------------------------------------------
// Level
// ------------------------------------------------------------------------------------------------------------
class Level : public Object, public std::enable_shared_from_this<Level>
{
public:
	Level(DirectX11& dx);
	virtual ~Level();

	// ------------------------------------------------------
	// Main
	// ------------------------------------------------------
	void SetWorld(std::shared_ptr<World> in);
	virtual std::shared_ptr<World> GetWorld() override;

protected:
	// ------------------------------------------------------
	// State
	// ------------------------------------------------------
	DirectX11* pDX = nullptr;

	std::shared_ptr<World> pOwningWorld = nullptr;
};

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
	void SetGroundType(const EGroundType& inGroundType);

protected:
	virtual void GenerateGroundLayer() = 0;
	virtual void GenerateEventLayer() = 0;
	virtual void GenerateItemLayer() {}
	virtual void GenerateBuildingLayer() {}
	virtual void GenerateCharacterLayer() {}
	virtual void GenerateEffectLayer() {}

public:
	void Init(const UINT16& x, const UINT16& y);
	void Activate();
	void Deactivate();
	bool MoveCenter(const int& x, const int& y);

	virtual void Tick(DirectX11& dx, float deltaTime) override;

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
	UINT16 GetSize() const noexcept;
	const UINT16& GetWidth() const noexcept;
	const UINT16& GetHeight() const noexcept;

	bool IsInScreen(const int& x, const int& y) const noexcept;
	bool IsInWorld(const int& x, const int& y) const noexcept;
	bool IsEmptyGround(const int& x, const int& y) const noexcept;

	void SetGroundLayerID(const EGroundId& groundType, const UINT16& x, const UINT16& y);
	void SetGroundLayerID(const EGroundId& groundType, const UINT16& inMinXY, const UINT16& inMaxXY, const UINT16& inConstantXY, bool bConstantHorizontal = false, INT16 inConstantXY2 = -1);
	const std::shared_ptr<GroundBase>& GetGroundLayerID(const int& x, const int& y) const;

	void SetEventLayerID(const EEventId& eventType, const UINT16& x, const UINT16& y);
	void SetEventLayerID(std::shared_ptr<EventBase>& inEventData, const UINT16& x, const UINT16& y);
	void SetEventLayerID(std::shared_ptr<EventBase>&& inEventData, const UINT16& x, const UINT16& y);
	const std::shared_ptr<EventBase>& GetEventLayerID(const int& x, const int& y) const;

	void SetItemLayerID(const EItemId& itemType, const UINT16& x, const UINT16& y);
	const std::shared_ptr<ItemBase>& GetItemLayerID(const int& x, const int& y) const;

	void SetCharacterLayerID(const ECharacterId& characterType, const UINT16& x, const UINT16& y);
	const std::shared_ptr<CharacterBase>& GetCharacterLayerID(const int& x, const int& y) const;

	const std::shared_ptr<CharacterBase>& GetCharacter2LayerID(const int& x, const int& y) const;
protected:
	DirectX::XMFLOAT2 WorldToScreen(const int& x, const int& y, const FVector2D& size);
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

	UINT16 width = 0;
	UINT16 height = 0;
protected:
	bool bInitialized = false;
	EGroundType mGroundType = EGroundType::Cave;

	// --------------------------
	// State : Display
	// --------------------------
	int startX = 0;
	int startY = 0;

	UINT16 currX = 0;
	UINT16 currY = 0;
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